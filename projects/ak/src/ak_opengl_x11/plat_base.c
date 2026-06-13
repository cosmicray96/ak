#include "ak/platform/plat_base.h"
#include "ak/debug.h"
#include "ak_opengl/gfx/gfx.h"
#include "ak_x11/platform/plat.h"

#include <glad/glad.h>

#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>

//===== ak_plat_ren =====//
#define s_init_width 800
#define s_init_height 600

//--- private ---//
struct ak_plat_base
{
  ak_alct alct;
  Display* d;
  Window wn;
  Atom wm_delete;
  GLXContext glx_ctx;
  GLXFBConfig fb;
};

//--- impl inter ---//
void
ak_plat_base_glmakecurrent(ak_plat_base* pb)
{
  typedef GLXContext (
    *glXCreateContextAttribsARBProc)(
    Display*,
    GLXFBConfig,
    GLXContext,
    Bool,
    const int*);
  glXCreateContextAttribsARBProc
    glXCreateContextAttribsARB =
      (glXCreateContextAttribsARBProc)
        glXGetProcAddressARB(
          (const GLubyte*)"glXCreateContex"
                          "tAttribsARB");
  ak_log_assert(
    glXCreateContextAttribsARB,
    "Cannot get "
    "glXCreateContextAttribsARBProc.");

  int ctx_attribs[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB,
    3,
    GLX_CONTEXT_MINOR_VERSION_ARB,
    3,
    GLX_CONTEXT_PROFILE_MASK_ARB,
    GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
    None
  };

  pb->glx_ctx = glXCreateContextAttribsARB(
    pb->d, pb->fb, 0, True, ctx_attribs);
  ak_log_assert(pb->glx_ctx,
                "Cannot create glXContext.");

  glXMakeCurrent(pb->d, pb->wn, pb->glx_ctx);

  if (!gladLoadGLLoader((
        GLADloadproc)glXGetProcAddressARB)) {
    ak_log_assert(
      false, "gladLoadGLLoader failed.");
  }

  glViewport(
    0, 0, s_init_width, s_init_height);
}

Display*
ak_plat_base_display(ak_plat_base* pb)
{
  return pb->d;
}
Window
ak_plat_base_window(ak_plat_base* pb)
{
  return pb->wn;
}
Atom
ak_plat_base_wm_delete(ak_plat_base* pb)
{
  return pb->wm_delete;
}

//--- public ---//
ak_plat_base*
ak_plat_base_startup(ak_alct alct)
{
  ak_plat_base* pb = ak_alct_alloc(
    alct, sizeof(ak_plat_base));
  pb->alct = alct;

  pb->d = XOpenDisplay(NULL);
  ak_log_assert(pb->d,
                "Cannot open X11 Display.");

  int screen = DefaultScreen(pb->d);

  {
    int glx_major = 0, glx_minor = 0;
    ak_log_assert(
      glXQueryVersion(
        pb->d, &glx_major, &glx_minor),
      "glXQueryVersion failed.");
    ak_log_assert(
      (glx_major > 1) ||
        (glx_major == 1 && glx_minor >= 3),
      "GLX 1.3 or higher required.");

    // GLX_ARB_create_context_profile
    // (implies GLX_ARB_create_context)
    const char* glx_exts =
      glXQueryExtensionsString(pb->d,
                               screen);
    ak_log_assert(
      glx_exts,
      "glXQueryExtensionsString failed.");

    // strstr is fine here; extension tokens
    // don't overlap in ways that cause false
    // positives
    ak_log_assert(
      strstr(
        glx_exts,
        "GLX_ARB_create_context_profile"),
      "GLX_ARB_create_context_profile "
      "required.");
  }

  GLXFBConfig* fbc = 0;
  GLXFBConfig fb = 0;
  XVisualInfo* vi = 0;
  {
    // Choose a GLX FBConfig for
    // OpenGL 3.3 core
    int fb_attribs[] = { GLX_X_RENDERABLE,
                         True,
                         GLX_DRAWABLE_TYPE,
                         GLX_WINDOW_BIT,
                         GLX_RENDER_TYPE,
                         GLX_RGBA_BIT,
                         GLX_X_VISUAL_TYPE,
                         GLX_TRUE_COLOR,
                         GLX_RED_SIZE,
                         8,
                         GLX_GREEN_SIZE,
                         8,
                         GLX_BLUE_SIZE,
                         8,
                         GLX_ALPHA_SIZE,
                         8,
                         GLX_DEPTH_SIZE,
                         24,
                         GLX_STENCIL_SIZE,
                         8,
                         GLX_DOUBLEBUFFER,
                         True,
                         None };

    int fbcount = 0;
    fbc = glXChooseFBConfig(
      pb->d, screen, fb_attribs, &fbcount);

    ak_log_assert(fbc,
                  "Cannot get glXFBConfig.");
    ak_log_assert(
      fbcount, "glXFBConfig count is zero.");

    // Pick first config
    fb = fbc[0];
    pb->fb = fb;

    vi = glXGetVisualFromFBConfig(pb->d, fb);
    ak_log_assert(vi,
                  "Cannot get VisualInfo "
                  "from FBConfig.");
  }

  uint32_t width = s_init_width;
  uint32_t height = s_init_height;
  {
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t border = 0;

    XSetWindowAttributes swa;
    swa.colormap = XCreateColormap(
      pb->d,
      RootWindow(pb->d, vi->screen),
      vi->visual,
      AllocNone);
    swa.event_mask = ExposureMask |
                     KeyPressMask |
                     StructureNotifyMask;

    pb->wn = XCreateWindow(
      pb->d,
      RootWindow(pb->d, vi->screen),
      x,
      y,
      width,
      height,
      border,
      vi->depth,
      InputOutput,
      vi->visual,
      CWColormap | CWEventMask,
      &swa);

    XFree(fbc);
    XFree(vi);
  }

  XStoreName(pb->d, pb->wn, "Window Title!");

  pb->wm_delete = XInternAtom(
    pb->d, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(
    pb->d, pb->wn, &pb->wm_delete, 1);
  XSelectInput(
    pb->d,
    pb->wn,
    ButtonPressMask | ButtonReleaseMask |
      KeyPressMask | KeyReleaseMask |
      PointerMotionMask | FocusChangeMask |
      StructureNotifyMask |
      VisibilityChangeMask);

  XMapWindow(pb->d, pb->wn);
  XFlush(pb->d);

  return pb;
}

void
ak_plat_base_shutdown(ak_plat_base* pb)
{
  glXMakeCurrent(pb->d, None, NULL);
  glXDestroyContext(pb->d, pb->glx_ctx);

  XDestroyWindow(pb->d, pb->wn);
  XCloseDisplay(pb->d);

  ak_alct_free(pb->alct, pb);
}

void
ak_plat_base_swapbuffer(ak_plat_base* pb)
{
  glXSwapBuffers(pb->d, pb->wn);
}

uint32_t
ak_plat_base_init_width(ak_plat_base* pb)
{
  return s_init_width;
}
uint32_t
ak_plat_base_init_height(ak_plat_base* pb)
{
  return s_init_height;
}
