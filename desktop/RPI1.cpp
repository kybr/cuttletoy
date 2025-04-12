#include "Window.hpp"

#include <cassert>

//#include <fcntl.h>
//#include <math.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//
//extern "C" {
//#include "revision.h"
//}

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "bcm_host.h"

#include <iostream> // fprintf

bool egl_good() {
  switch (eglGetError()) {
    case EGL_SUCCESS: return true;
    case EGL_NOT_INITIALIZED: fprintf(stderr, "EGL_NOT_INITIALIZED\n"); break;
    case EGL_BAD_ACCESS: fprintf(stderr, "EGL_BAD_ACCESS\n"); break;
    case EGL_BAD_ALLOC: fprintf(stderr, "EGL_BAD_ALLOC\n"); break;
    case EGL_BAD_ATTRIBUTE: fprintf(stderr, "EGL_BAD_ATTRIBUTE\n"); break;
    case EGL_BAD_CONTEXT: fprintf(stderr, "EGL_BAD_CONTEXT\n"); break;
    case EGL_BAD_CONFIG: fprintf(stderr, "EGL_BAD_CONFIG\n"); break;
    case EGL_BAD_CURRENT_SURFACE: fprintf(stderr, "EGL_BAD_CURRENT_SURFACE\n"); break;
    case EGL_BAD_DISPLAY: fprintf(stderr, "EGL_BAD_DISPLAY\n"); break;
    case EGL_BAD_SURFACE: fprintf(stderr, "EGL_BAD_SURFACE\n"); break;
    case EGL_BAD_MATCH: fprintf(stderr, "EGL_BAD_MATCH\n"); break;
    case EGL_BAD_PARAMETER: fprintf(stderr, "EGL_BAD_PARAMETER\n"); break;
    case EGL_BAD_NATIVE_PIXMAP: fprintf(stderr, "EGL_BAD_NATIVE_PIXMAP\n"); break;
    case EGL_BAD_NATIVE_WINDOW: fprintf(stderr, "EGL_BAD_NATIVE_WINDOW\n"); break;
    case EGL_CONTEXT_LOST: fprintf(stderr, "EGL_CONTEXT_LOST\n"); break;
    default: fprintf(stderr, "UNKNOWN EGL ERROR\n"); break;
  }
  return false;
}
// assert(egl_good();


struct WindowImp {
  uint32_t width;
  uint32_t height;
  EGLContext context;
  EGLDisplay display;
  EGLSurface surface;

  WindowImp() {
    bcm_host_init();

    printf("Legacy RPI1 OpenGL backend\n");

    width = 0;
    height = 0;
    memset(&context, 0, sizeof(EGLContext));
    memset(&display, 0, sizeof(EGLDisplay));
    memset(&surface, 0, sizeof(EGLSurface));

    int32_t success = 0;
    EGLBoolean result;
    EGLint num_config;
  
    static EGL_DISPMANX_WINDOW_T nativewindow;
  
    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;
  
    static const EGLint attribute_list[] = {EGL_RED_SIZE,     8,
                                            EGL_GREEN_SIZE,   8,
                                            EGL_BLUE_SIZE,    8,
                                            EGL_ALPHA_SIZE,   8,
                                            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                                            EGL_NONE};
  
    static const EGLint context_attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2,
                                                EGL_NONE};
    EGLConfig config;
  
    // get an EGL display connection
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(display != EGL_NO_DISPLAY);
    assert(egl_good());
  
    // initialize the EGL display connection
    result = eglInitialize(display, NULL, NULL);
    assert(EGL_FALSE != result);
    assert(egl_good());
  
    // get an appropriate EGL frame buffer configuration
    result = eglChooseConfig(display, attribute_list, &config, 1, &num_config);
    assert(EGL_FALSE != result);
    assert(egl_good());
  
    // get an appropriate EGL frame buffer configuration
    result = eglBindAPI(EGL_OPENGL_ES_API);
    assert(EGL_FALSE != result);
    assert(egl_good());
  
    // create an EGL rendering context
    context =
        eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
    assert(context != EGL_NO_CONTEXT);
    assert(egl_good());
  
    // create an EGL window surface
    success = graphics_get_display_size(0 /* LCD */, &width, &height);
    assert(success >= 0);
  
    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = width;
    dst_rect.height = height;
  
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = width << 16;
    src_rect.height = height << 16;
  
    dispman_display = vc_dispmanx_display_open(0 /* LCD */);
    dispman_update = vc_dispmanx_update_start(0);
  
    dispman_element = vc_dispmanx_element_add(
        dispman_update, dispman_display, 0 /*layer*/, &dst_rect, 0 /*src*/,
        &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0 /*clamp*/,
        (DISPMANX_TRANSFORM_T)0 /*transform*/);
  
    nativewindow.element = dispman_element;
    nativewindow.width = width;
    nativewindow.height = height;
    vc_dispmanx_update_submit_sync(dispman_update);
  
    assert(egl_good());
  
    surface = eglCreateWindowSurface(display, config, &nativewindow, NULL);
    assert(surface != EGL_NO_SURFACE);
    assert(egl_good());
  
    // connect the context to the surface
    result = eglMakeCurrent(display, surface, surface, context);
    assert(EGL_FALSE != result);
    assert(egl_good());

    printf("Window is %d by %d\n", width, height);
  }

  ~WindowImp() {
    bcm_host_deinit();
  }

  void swap() { 
    eglSwapBuffers(display, surface);
  }

  void size(int& width, int& height) {
    width = this->width;
    height = this->height;
  }

  bool done() { return false; } 
};

Window::Window() { i = new WindowImp(); }
Window::~Window() { delete i; }
void Window::swap() { i->swap(); } 
void Window::size(int& width, int& height) { i->size(width, height); }
bool Window::done() { return i->done(); }
