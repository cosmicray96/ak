#include "ak_android/android.h"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android_native_app_glue.h>

static EGLDisplay display = EGL_NO_DISPLAY;
static EGLSurface surface = EGL_NO_SURFACE;
static EGLContext context = EGL_NO_CONTEXT;

static void
init_display(struct android_app* app)
{
  EGLint attribs[] = {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_BLUE_SIZE,    8,
    EGL_GREEN_SIZE,   8,
    EGL_RED_SIZE,     8,
    EGL_NONE
  };

  EGLint w, h, format, numConfigs;
  EGLConfig config;

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
term_display()
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

static int ready = 0;

static void
ak_handle_cmd(struct android_app* app,
              int32_t cmd)
{
  switch (cmd) {
    case APP_CMD_INIT_WINDOW:
      init_display(app);
      ready = 1;
      break;
    case APP_CMD_TERM_WINDOW:
      term_display();
      ready = 0;
      break;
  }
}

void
android_main(struct android_app* app)
{
  ak_android_app = app;
  app->onAppCmd = ak_handle_cmd;

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
        term_display();
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
