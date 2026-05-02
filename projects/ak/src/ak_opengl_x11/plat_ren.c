#include "ak/platform/plat_ren.h"
#include "ak/debug.h"
#include "ak_x11/platform/itn.h"

#include <glad/glad.h>

#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

//===== ak_plat_ren =====//
#define s_init_width 200
#define s_init_height 150

//--- private ---//
struct ak_plat_ren
{
  ak_alct alct;
  Display* d;
  Window wn;
  Atom wm_delete;
  GLXContext glx_ctx;
};

//--- public ---//
ak_plat_ren*
ak_plat_ren_startup(ak_alct alct)
{
  ak_plat_ren* pr =
    ak_alct_alloc(alct, sizeof(ak_plat_ren));
  pr->alct = alct;

  pr->d = XOpenDisplay(NULL);
  ak_assert(pr->d);

  int screen = DefaultScreen(pr->d);

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
      pr->d, screen, fb_attribs, &fbcount);

    ak_assert(fbc);
    ak_assert(fbcount);

    // Pick first config
    fb = fbc[0];

    vi = glXGetVisualFromFBConfig(pr->d, fb);
    ak_assert(vi);
  }

  uint32_t width = s_init_width;
  uint32_t height = s_init_height;
  {
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t border = 0;

    XSetWindowAttributes swa;
    swa.colormap = XCreateColormap(
      pr->d,
      RootWindow(pr->d, vi->screen),
      vi->visual,
      AllocNone);
    swa.event_mask = ExposureMask |
                     KeyPressMask |
                     StructureNotifyMask;

    pr->wn = XCreateWindow(
      pr->d,
      RootWindow(pr->d, vi->screen),
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

  XStoreName(pr->d, pr->wn, "Window Title!");

  pr->wm_delete = XInternAtom(
    pr->d, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(
    pr->d, pr->wn, &pr->wm_delete, 1);
  XSelectInput(
    pr->d,
    pr->wn,
    ButtonPressMask | ButtonReleaseMask |
      KeyPressMask | KeyReleaseMask |
      PointerMotionMask | FocusChangeMask |
      StructureNotifyMask |
      VisibilityChangeMask);

  XMapWindow(pr->d, pr->wn);
  XFlush(pr->d);

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
    ak_assert(glXCreateContextAttribsARB);

    int ctx_attribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB,
      3,
      GLX_CONTEXT_MINOR_VERSION_ARB,
      3,
      GLX_CONTEXT_PROFILE_MASK_ARB,
      GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
      None
    };

    pr->glx_ctx = glXCreateContextAttribsARB(
      pr->d, fb, 0, True, ctx_attribs);
    ak_assert(pr->glx_ctx);

    glXMakeCurrent(
      pr->d, pr->wn, pr->glx_ctx);

    if (!gladLoadGLLoader(
          (GLADloadproc)
            glXGetProcAddressARB)) {
      ak_assert(false);
    }

    glViewport(0, 0, width, height);
  }

  return pr;
}

void
ak_plat_ren_shutdown(ak_plat_ren* pr)
{
  glXMakeCurrent(pr->d, None, NULL);
  glXDestroyContext(pr->d, pr->glx_ctx);

  XDestroyWindow(pr->d, pr->wn);
  XCloseDisplay(pr->d);

  ak_alct_free(pr->alct, pr);
}

void
ak_plat_ren_swapbuffer(ak_plat_ren* pr)
{
  glXSwapBuffers(pr->d, pr->wn);
}

Display*
ak_plat_ren_display(ak_plat_ren* pr)
{
  return pr->d;
}
Window
ak_plat_ren_window(ak_plat_ren* pr)
{
  return pr->wn;
}
Atom
ak_plat_ren_wm_delete(ak_plat_ren* pr)
{
  return pr->wm_delete;
}

int32_t
ak_plat_ren_init_width(ak_plat_ren* pr)
{
  return s_init_width;
}

int32_t
ak_plat_ren_init_height(ak_plat_ren* pr)
{
  return s_init_height;
}
