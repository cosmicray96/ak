#include "ak/ak.h"
#include "ak_android/android.h"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android_native_app_glue.h>

static EGLDisplay display = EGL_NO_DISPLAY;
static EGLSurface surface = EGL_NO_SURFACE;
static EGLContext context = EGL_NO_CONTEXT;
static EGLConfig config;
static EGLint format;

static void
gfx_startup(struct android_app* app)
{
  EGLint attribs[] = {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_BLUE_SIZE,    8,
    EGL_GREEN_SIZE,   8,
    EGL_RED_SIZE,     8,
    EGL_NONE
  };

  EGLint numConfigs;

  display =
    eglGetDisplay(EGL_DEFAULT_DISPLAY);
  eglInitialize(display, 0, 0);
  eglChooseConfig(display,
                  attribs,
                  &config,
                  1,
                  &numConfigs);
  eglGetConfigAttrib(display,
                     config,
                     EGL_NATIVE_VISUAL_ID,
                     &format);
}

static void
gfx_shutdown(struct android_app* app)
{
  if (display != EGL_NO_DISPLAY) {
    eglMakeCurrent(display,
                   EGL_NO_SURFACE,
                   EGL_NO_SURFACE,
                   EGL_NO_CONTEXT);
    if (context != EGL_NO_CONTEXT)
      eglDestroyContext(display, context);
    if (surface != EGL_NO_SURFACE)
      eglDestroySurface(display, surface);
    eglTerminate(display);
  }
  display = EGL_NO_DISPLAY;
  surface = EGL_NO_SURFACE;
  context = EGL_NO_CONTEXT;
}

static void
display_make(struct android_app* app)
{
  ANativeWindow_setBuffersGeometry(
    app->window, 0, 0, format);

  surface = eglCreateWindowSurface(
    display, config, app->window, NULL);
  EGLint ctx_attribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
  };
  context = eglCreateContext(
    display, config, NULL, ctx_attribs);
  eglMakeCurrent(
    display, surface, surface, context);
}

static void
display_destroy()
{
  eglMakeCurrent(display,
                 EGL_NO_SURFACE,
                 EGL_NO_SURFACE,
                 EGL_NO_CONTEXT);
  if (surface != EGL_NO_SURFACE) {
    eglDestroySurface(display, surface);
    surface = EGL_NO_SURFACE;
  }
}

static int ready = 0;

static void
ak_handle_cmd_old(struct android_app* app,
                  int32_t cmd)
{
  switch (cmd) {
    case APP_CMD_START:
      gfx_startup(app);
      break;
    case APP_CMD_STOP:
      gfx_shutdown(app);
      break;
    case APP_CMD_INIT_WINDOW:
      display_make(app);
      ready = 1;
      break;
    case APP_CMD_TERM_WINDOW:
      display_destroy();
      ready = 0;
      break;
  }
}

void
android_main(struct android_app* app)
{
  ak_android_app_set(app);

  ak();

  while (1) {
    int events;
    struct android_poll_source* source;

    while (ALooper_pollOnce(
             ready ? 0 : -1,
             NULL,
             &events,
             (void**)&source) >= 0) {
      if (source)
        source->process(app, source);
      if (app->destroyRequested) {
        display_destroy();
        gfx_shutdown(app);
        return;
      }
    }

    if (ready) {
      glClearColor(
        0.39f, 0.58f, 0.93f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      eglSwapBuffers(display, surface);
    }
  }
}
