#include "ak/platform/plat_base.h"
#include "ak/app/event.h"
#include "ak/core/async/cond.h"
#include "ak/core/async/mutex.h"
#include "ak/system/render.h"
#include "ak_android/android.h"
#include <EGL/egl.h>
#include <android/native_window.h>

struct ak_plat_base
{
  ak_alct alct;

  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;
  EGLConfig config;
  EGLint format;

  uint32_t width;
  uint32_t height;

  ak_mutex m_sfc;
  bool sfc_inited;
  bool sfc_ready;
};
static ak_plat_base* s_pb = 0;

ak_plat_base*
ak_plat_base_startup(ak_alct alct)
{
  ak_plat_base* pb = ak_alct_alloc(
    alct, sizeof(ak_plat_base));
  s_pb = pb;
  pb->alct = alct;

  pb->m_sfc = ak_mutex_make();
  pb->sfc_inited = false;

  ak_plat_base_render_lock(s_pb);
  return pb;
}

void
ak_plat_base_shutdown(ak_plat_base* pb)
{
  ak_mutex_destroy(&pb->m_sfc);
  ak_alct_free(pb->alct, pb);
}

void
ak_plat_base_swapbuffer(ak_plat_base* pb)
{
  return;
  eglSwapBuffers(pb->display, pb->surface);
}

void
ak_plat_base_eventflush(ak_plat_base* pb,
                        ak_app_eq* eq)
{
  int timeout = 0;
  int events;
  struct android_poll_source* source;
  while (ALooper_pollOnce(timeout,
                          NULL,
                          &events,
                          (void**)&source) >=
         0) {

    if (source)
      source->process(aks_android_app,
                      source);
    if (aks_android_app->destroyRequested) {
      ak_evt e = { .type = ak_evt_type_pgm,
                   .pgm = ak_pgm_exit_req };
      ak_app_eq_push(eq, e);
    }
  }
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
ak_plat_base_render_trylock(ak_plat_base* pb)
{
  return false;
  return ak_mutex_trylock(&pb->m_sfc);
}
void
ak_plat_base_render_lock(ak_plat_base* pb)
{
  return;
  ak_mutex_lock(&pb->m_sfc);
}
void
ak_plat_base_render_unlock(ak_plat_base* pb)
{
  return;
  ak_mutex_unlock(&pb->m_sfc);
}

void
ak_plat_base_glctx_startup(ak_plat_base* pb)
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
ak_plat_base_glctx_shutdown(ak_plat_base* pb)
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

static void
surface_make(void* ctx)
{
  ak_plat_base* pb = ctx;
  ANativeWindow_setBuffersGeometry(
    aks_android_app->window,
    0,
    0,
    pb->format);

  pb->surface = eglCreateWindowSurface(
    pb->display,
    pb->config,
    aks_android_app->window,
    NULL);
  eglMakeCurrent(pb->display,
                 pb->surface,
                 pb->surface,
                 pb->context);
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

void
ak_handle_cmd(struct android_app* app,
              int32_t cmd)
{
  return;
  switch (cmd) {
    case APP_CMD_INIT_WINDOW: {
      ak_renderer* r = ak_renderer_get();
      ak_renderer_run_fn(
        r, &surface_make, s_pb);
      s_pb->sfc_ready = true;
      ak_plat_base_render_unlock(s_pb);
      break;
    }
    case APP_CMD_TERM_WINDOW: {
      ak_renderer* r = ak_renderer_get();
      ak_plat_base_render_lock(s_pb);
      s_pb->sfc_ready = false;
      ak_renderer_run_fn(
        r, &surface_destroy, s_pb);
      break;
    }
  }
}
