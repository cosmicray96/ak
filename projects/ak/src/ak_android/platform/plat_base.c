#include "ak/platform/plat_base.h"
#include "ak/app/event.h"
#include "ak/res/core.h"
#include "ak_android/android.h"
#include "ak_android/gfx/rctx.h"
#include "ak_android/platform/plat_base.h"
#include "ak_opengl/platform/plat_base.h"

#include <EGL/egl.h>
#include <android/native_window.h>

//===== ak_plat_base =====//
#define akd_width_init 100
#define akd_height_init 100

//--- private ---//
struct ak_plat_base
{
  ak_alct alct;

  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;
  EGLContext loader_context;
  EGLSurface loader_surface;
  EGLConfig config;
  EGLint format;

  uint32_t width;
  uint32_t height;

  bool surface_ready;

  ak_rctx* rctx;

  ak_app_eq* eq;
  ak_resreg* rr;
};
static ak_plat_base* s_pb = 0;

static void
surface_make(void* ctx)
{
  ak_plat_base* pb = ctx;
  ANativeWindow_setBuffersGeometry(
    ak_android_app()->window,
    0,
    0,
    pb->format);

  pb->surface = eglCreateWindowSurface(
    pb->display,
    pb->config,
    ak_android_app()->window,
    NULL);
  eglMakeCurrent(pb->display,
                 pb->surface,
                 pb->surface,
                 pb->context);

  EGLint width, height;
  eglQuerySurface(pb->display,
                  pb->surface,
                  EGL_WIDTH,
                  &width);
  eglQuerySurface(pb->display,
                  pb->surface,
                  EGL_HEIGHT,
                  &height);
  pb->width = width;
  pb->height = height;

  ak_evt e = {
    .type = ak_evttype_win,
    .win = { .type = ak_evtwintype_resize,
             .resize = { .w = width,
                         .h = height } }
  };
  ak_app_eq_push(pb->eq, &e);
}

static void
surface_destroy(void* ctx)
{
  ak_plat_base* pb = ctx;
  eglMakeCurrent(pb->display,
                 EGL_NO_SURFACE,
                 EGL_NO_SURFACE,
                 EGL_NO_CONTEXT);
  if (pb->surface != EGL_NO_SURFACE) {
    eglDestroySurface(pb->display,
                      pb->surface);
    pb->surface = EGL_NO_SURFACE;
  }
}
static void
handle_cmd(struct android_app* app,
           int32_t cmd)
{
  switch (cmd) {
    case APP_CMD_INIT_WINDOW: {
      if (!s_pb->surface_ready) {
        surface_make(s_pb);
        s_pb->surface_ready = true;
      }
      break;
    }
    case APP_CMD_TERM_WINDOW: {
      if (s_pb->surface_ready) {
        s_pb->surface_ready = false;
        surface_destroy(s_pb);
      }
      break;
    }
  }
}

//--- internal ---//
ak_plat_base*
ak_plat_base_startup(ak_app_eq* eq,
                     ak_resreg* rr,
                     ak_alct alct)
{
  ak_plat_base* pb = ak_alct_alloc(
    alct, sizeof(ak_plat_base));
  s_pb = pb;
  pb->surface_ready = false;

  pb->eq = eq;
  pb->rr = rr;
  pb->alct = alct;

  pb->display = EGL_NO_DISPLAY;
  pb->surface = EGL_NO_SURFACE;
  pb->context = EGL_NO_CONTEXT;
  pb->loader_surface = EGL_NO_SURFACE;
  pb->loader_context = EGL_NO_CONTEXT;

  ak_android_app()->onAppCmd = &handle_cmd;

  pb->width = akd_width_init;
  pb->height = akd_height_init;

  ak_opengl_plat_base_glctx_startup(pb);

  while (!pb->surface_ready) {
    ak_plat_base_eventflush(pb);
  }

  pb->rctx =
    ak_android_rctx_startup(pb, rr, alct);

  return pb;
}

void
ak_plat_base_shutdown(ak_plat_base* pb)
{
  ak_android_rctx_shutdown(pb->rctx);
  ak_opengl_plat_base_glctx_shutdown(pb);
  pb->eq = 0;
  ak_alct_free(pb->alct, pb);
}

void
ak_opengl_plat_base_swapbuffer(
  ak_plat_base* pb)
{
  eglSwapBuffers(pb->display, pb->surface);
}

void
ak_plat_base_eventflush(ak_plat_base* pb)
{
  int events;
  struct android_poll_source* source;
  while (ALooper_pollOnce(
           pb->surface_ready ? 0 : -1,
           NULL,
           &events,
           (void**)&source) >= 0) {

    if (source)
      source->process(ak_android_app(),
                      source);
    if (ak_android_app()->destroyRequested) {
      ak_evt e = { .type = ak_evttype_pgm,
                   .pgm = ak_evtpgm_exit };
      ak_app_eq_push(pb->eq, &e);
    }
  }
}

ak_rctx*
ak_plat_base_rctx(ak_plat_base* pb)
{
  return pb->rctx;
}

uint32_t
ak_plat_base_width(ak_plat_base* pb)
{
  return pb->width;
}
uint32_t
ak_plat_base_height(ak_plat_base* pb)
{
  return pb->height;
}

bool
ak_android_plat_base_surface_ready(
  ak_plat_base* pb)
{
  return pb->surface_ready;
}

void
ak_opengl_plat_base_glctx_startup(
  ak_plat_base* pb)
{
  EGLint attribs[] = { EGL_SURFACE_TYPE,
                       EGL_WINDOW_BIT,
                       EGL_RENDERABLE_TYPE,
                       EGL_OPENGL_ES3_BIT,
                       EGL_RED_SIZE,
                       8,
                       EGL_GREEN_SIZE,
                       8,
                       EGL_BLUE_SIZE,
                       8,
                       EGL_ALPHA_SIZE,
                       8,
                       EGL_NONE };

  EGLint numConfigs;

  pb->display =
    eglGetDisplay(EGL_DEFAULT_DISPLAY);
  eglInitialize(pb->display, 0, 0);
  eglBindAPI(EGL_OPENGL_ES_API);
  eglChooseConfig(pb->display,
                  attribs,
                  &pb->config,
                  1,
                  &numConfigs);
  eglGetConfigAttrib(pb->display,
                     pb->config,

                     EGL_NATIVE_VISUAL_ID,
                     &pb->format);
  EGLint ctx_attribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE
  };
  pb->context =
    eglCreateContext(pb->display,
                     pb->config,
                     NULL,
                     ctx_attribs);
}

void
ak_opengl_plat_base_glctx_shutdown(
  ak_plat_base* pb)
{
  if (pb->display != EGL_NO_DISPLAY) {
    eglMakeCurrent(pb->display,
                   EGL_NO_SURFACE,
                   EGL_NO_SURFACE,
                   EGL_NO_CONTEXT);
    if (pb->context != EGL_NO_CONTEXT)
      eglDestroyContext(pb->display,
                        pb->context);
    if (pb->surface != EGL_NO_SURFACE)
      eglDestroySurface(pb->display,
                        pb->surface);
    eglTerminate(pb->display);
  }
  pb->display = EGL_NO_DISPLAY;
  pb->surface = EGL_NO_SURFACE;
  pb->context = EGL_NO_CONTEXT;
}

void
ak_android_plat_base_rctx_loader_startup(
  ak_plat_base* pb)
{
  eglBindAPI(EGL_OPENGL_ES_API);

  EGLint pbuffer_attribs[] = {
    EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE
  };
  pb->loader_surface =
    eglCreatePbufferSurface(pb->display,
                            pb->config,
                            pbuffer_attribs);

  EGLint ctx_attribs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE
  };

  pb->loader_context =
    eglCreateContext(pb->display,
                     pb->config,
                     pb->context,
                     ctx_attribs);

  eglMakeCurrent(pb->display,
                 pb->loader_surface,
                 pb->loader_surface,
                 pb->loader_context);
}

void
ak_android_plat_base_rctx_loader_shutdown(
  ak_plat_base* pb)
{
  eglMakeCurrent(pb->display,
                 EGL_NO_SURFACE,
                 EGL_NO_SURFACE,
                 EGL_NO_CONTEXT);

  if (pb->loader_context != EGL_NO_CONTEXT) {
    eglDestroyContext(pb->display,
                      pb->loader_context);
    pb->loader_context = EGL_NO_CONTEXT;
  }
  if (pb->loader_surface != EGL_NO_SURFACE) {
    eglDestroySurface(pb->display,
                      pb->loader_surface);
    pb->loader_surface = EGL_NO_SURFACE;
  }
}
