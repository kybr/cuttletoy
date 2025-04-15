
#include <assert.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"
#include "bcm_host.h"
#include "revision.h"

#include <time.h>

#include <lo/lo.h>

#define GLSL_SIZE (65000)

#define ASSERT(target, value, message) do { \
  if (target != value) { \
    printf(message); \
    exit(1); \
  } \
} while (0)

typedef struct {
  int x, y;
  uint32_t screen_width;
  uint32_t screen_height;
  // OpenGL|ES objects
  EGLDisplay display;
  EGLSurface surface;
  EGLContext context;

  GLuint verbose;
  GLuint vshader;
  GLuint fshader;
  GLuint mshader;
  GLuint program_julia;
  GLuint program_mandelbrot;
  GLuint tex_fb;
  GLuint tex;
  GLuint buf;
  // julia attribs
  GLuint unif_color, attr_vertex, unif_scale, unif_offset, unif_tex,
      unif_centre;
  // mandelbrot attribs
  GLuint attr_vertex2, unif_scale2, unif_offset2, unif_centre2;

  int should_recompile_glsl;
  char glsl[GLSL_SIZE];
} STATE;

#define check() assert(glGetError() == 0)

static void showlog(GLint shader) {
  // Prints the compile log for a shader
  char log[1024];
  glGetShaderInfoLog(shader, sizeof log, NULL, log);
  printf("%d:shader:\n%s\n", shader, log);
}

static void showprogramlog(GLint shader) {
  // Prints the information log for a program object
  char log[1024];
  glGetProgramInfoLog(shader, sizeof log, NULL, log);
  printf("%d:program:\n%s\n", shader, log);
}

/***********************************************************
 * Name: init_ogl
 *
 * Arguments:
 *       STATE *state - holds OGLES model info
 *
 * Description: Sets the display, OpenGL|ES context and screen stuff
 *
 * Returns: void
 *
 ***********************************************************/

static void init_ogl(STATE *state) {
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
  state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  assert(state->display != EGL_NO_DISPLAY);
  check();

  // initialize the EGL display connection
  result = eglInitialize(state->display, NULL, NULL);
  assert(EGL_FALSE != result);
  check();

  // get an appropriate EGL frame buffer configuration
  result =
      eglChooseConfig(state->display, attribute_list, &config, 1, &num_config);
  assert(EGL_FALSE != result);
  check();

  // get an appropriate EGL frame buffer configuration
  result = eglBindAPI(EGL_OPENGL_ES_API);
  assert(EGL_FALSE != result);
  check();

  // create an EGL rendering context
  state->context = eglCreateContext(state->display, config, EGL_NO_CONTEXT,
                                    context_attributes);
  assert(state->context != EGL_NO_CONTEXT);
  check();

  // create an EGL window surface
  success = graphics_get_display_size(0 /* LCD */, &state->screen_width,
                                      &state->screen_height);
  assert(success >= 0);

  dst_rect.x = 0;
  dst_rect.y = 0;
  dst_rect.width = state->screen_width;
  dst_rect.height = state->screen_height;

  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.width = state->screen_width << 16;
  src_rect.height = state->screen_height << 16;

  dispman_display = vc_dispmanx_display_open(0 /* LCD */);
  dispman_update = vc_dispmanx_update_start(0);

  dispman_element = vc_dispmanx_element_add(
      dispman_update, dispman_display, 0 /*layer*/, &dst_rect, 0 /*src*/,
      &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0 /*clamp*/,
      0 /*transform*/);

  nativewindow.element = dispman_element;
  nativewindow.width = state->screen_width;
  nativewindow.height = state->screen_height;
  vc_dispmanx_update_submit_sync(dispman_update);

  check();

  state->surface =
      eglCreateWindowSurface(state->display, config, &nativewindow, NULL);
  assert(state->surface != EGL_NO_SURFACE);
  check();

  // connect the context to the surface
  result = eglMakeCurrent(state->display, state->surface, state->surface,
                          state->context);
  assert(EGL_FALSE != result);
  check();

  // Set background color and clear buffers
  glClearColor(0.15f, 0.25f, 0.35f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  check();
}

static void init_shaders(STATE *state) {
  static const GLfloat vertex_data[] = {-1.0, -1.0, 1.0, 1.0, 1.0, -1.0,
                                        1.0,  1.0,  1.0, 1.0, 1.0, 1.0,
                                        -1.0, 1.0,  1.0, 1.0};
  const GLchar *vshader_source =
      "attribute vec4 vertex;"
      "varying vec2 tcoord;"
      "void main(void) {"
      " vec4 pos = vertex;"
      " gl_Position = pos;"
      " tcoord = vertex.xy*0.5+0.5;"
      "}";

  // Mandelbrot
  const GLchar *mandelbrot_fshader_source =
      "uniform vec4 color;"
      "uniform vec2 scale;"
      "uniform vec2 centre;"
      "varying vec2 tcoord;"
      "void main(void) {"
      "  float intensity;"
      "  vec4 color2;"
      "  float cr=(gl_FragCoord.x-centre.x)*scale.x;"
      "  float ci=(gl_FragCoord.y-centre.y)*scale.y;"
      "  float ar=cr;"
      "  float ai=ci;"
      "  float tr,ti;"
      "  float col=0.0;"
      "  float p=0.0;"
      "  int i=0;"
      "  for(int i2=1;i2<16;i2++)"
      "  {"
      "    tr=ar*ar-ai*ai+cr;"
      "    ti=2.0*ar*ai+ci;"
      "    p=tr*tr+ti*ti;"
      "    ar=tr;"
      "    ai=ti;"
      "    if (p>16.0)"
      "    {"
      "      i=i2;"
      "      break;"
      "    }"
      "  }"
      "  color2 = vec4(float(i)*0.0625,0,0,1);"
      "  gl_FragColor = color2;"
      "}";

  // Julia
  const GLchar *julia_fshader_source =
      "uniform vec4 color;"
      "uniform vec2 scale;"
      "uniform vec2 centre;"
      "uniform vec2 offset;"
      "varying vec2 tcoord;"
      "uniform sampler2D tex;"
      "void main(void) {"
      "  float intensity;"
      "  vec4 color2;"
      "  float ar=(gl_FragCoord.x-centre.x)*scale.x;"
      "  float ai=(gl_FragCoord.y-centre.y)*scale.y;"
      "  float cr=(offset.x-centre.x)*scale.x;"
      "  float ci=(offset.y-centre.y)*scale.y;"
      "  float tr,ti;"
      "  float col=0.0;"
      "  float p=0.0;"
      "  int i=0;"
      "  vec2 t2;"
      "  t2.x=tcoord.x+(offset.x-centre.x)*(0.5/centre.y);"
      "  t2.y=tcoord.y+(offset.y-centre.y)*(0.5/centre.x);"
      "  for(int i2=1;i2<16;i2++)"
      "  {"
      "    tr=ar*ar-ai*ai+cr;"
      "    ti=2.0*ar*ai+ci;"
      "    p=tr*tr+ti*ti;"
      "    ar=tr;"
      "    ai=ti;"
      "    if (p>16.0)"
      "    {"
      "      i=i2;"
      "      break;"
      "    }"
      "  }"
      "  color2 = vec4(0,float(i)*0.0625,0,1);"
      "  color2 = color2+texture2D(tex,t2);"
      "  gl_FragColor = color2;"
      "}";

  state->vshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(state->vshader, 1, &vshader_source, 0);
  glCompileShader(state->vshader);
  check();

  if (state->verbose) showlog(state->vshader);

  state->fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(state->fshader, 1, &julia_fshader_source, 0);
  glCompileShader(state->fshader);
  check();

  if (state->verbose) showlog(state->fshader);

  state->mshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(state->mshader, 1, &mandelbrot_fshader_source, 0);
  glCompileShader(state->mshader);
  check();

  if (state->verbose) showlog(state->mshader);

  // julia
  state->program_julia = glCreateProgram();
  glAttachShader(state->program_julia, state->vshader);
  glAttachShader(state->program_julia, state->fshader);
  glLinkProgram(state->program_julia);
  check();

  if (state->verbose) showprogramlog(state->program_julia);

  state->attr_vertex = glGetAttribLocation(state->program_julia, "vertex");
  state->unif_color = glGetUniformLocation(state->program_julia, "color");
  state->unif_scale = glGetUniformLocation(state->program_julia, "scale");
  state->unif_offset = glGetUniformLocation(state->program_julia, "offset");
  state->unif_tex = glGetUniformLocation(state->program_julia, "tex");
  state->unif_centre = glGetUniformLocation(state->program_julia, "centre");

  // mandelbrot
  state->program_mandelbrot = glCreateProgram();
  glAttachShader(state->program_mandelbrot, state->vshader);
  glAttachShader(state->program_mandelbrot, state->mshader);
  glLinkProgram(state->program_mandelbrot);
  check();

  if (state->verbose) showprogramlog(state->program_mandelbrot);

  state->attr_vertex2 = glGetAttribLocation(state->program_mandelbrot, "vertex");
  state->unif_scale2 = glGetUniformLocation(state->program_mandelbrot, "scale");
  state->unif_offset2 = glGetUniformLocation(state->program_mandelbrot, "offset");
  state->unif_centre2 = glGetUniformLocation(state->program_mandelbrot, "centre");
  check();

  glClearColor(0.0, 1.0, 1.0, 1.0);

  glGenBuffers(1, &state->buf);

  check();

  // Prepare a texture image
  glGenTextures(1, &state->tex);
  check();
  glBindTexture(GL_TEXTURE_2D, state->tex);
  check();
  // glActiveTexture(0)
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, state->screen_width,
               state->screen_height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 0);
  check();
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  check();
  // Prepare a framebuffer for rendering
  glGenFramebuffers(1, &state->tex_fb);
  check();
  glBindFramebuffer(GL_FRAMEBUFFER, state->tex_fb);
  check();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         state->tex, 0);
  check();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  check();
  // Prepare viewport
  glViewport(0, 0, state->screen_width, state->screen_height);
  check();

  // Upload vertex data to a buffer
  glBindBuffer(GL_ARRAY_BUFFER, state->buf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
               GL_STATIC_DRAW);
  glVertexAttribPointer(state->attr_vertex, 4, GL_FLOAT, 0, 16, 0);
  glEnableVertexAttribArray(state->attr_vertex);
  glVertexAttribPointer(state->attr_vertex2, 4, GL_FLOAT, 0, 16, 0);
  glEnableVertexAttribArray(state->attr_vertex2);

  check();
}

static void draw_mandelbrot_to_texture(STATE *state, GLfloat cx,
                                       GLfloat cy, GLfloat scale) {
  // Draw the mandelbrot to a texture
  glBindFramebuffer(GL_FRAMEBUFFER, state->tex_fb);
  check();
  glBindBuffer(GL_ARRAY_BUFFER, state->buf);

  glUseProgram(state->program_mandelbrot);
  check();

  glUniform2f(state->unif_scale2, scale, scale);
  glUniform2f(state->unif_centre2, cx, cy);
  check();
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  check();

  glFlush();
  glFinish();
  check();
}

static void draw(STATE *state, GLfloat cx, GLfloat cy,
                           GLfloat scale, GLfloat x, GLfloat y) {
  // Now render to the main frame buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // Clear the background (not really necessary I suppose)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  check();

  glBindBuffer(GL_ARRAY_BUFFER, state->buf);
  check();
  glUseProgram(state->program_julia);
  check();
  glBindTexture(GL_TEXTURE_2D, state->tex);
  check();
  glUniform4f(state->unif_color, 0.5, 0.5, 0.8, 1.0);
  glUniform2f(state->unif_scale, scale, scale);
  glUniform2f(state->unif_offset, x, y);
  glUniform2f(state->unif_centre, cx, cy);
  glUniform1i(state->unif_tex, 0);  // I don't really understand this part,
                                    // perhaps it relates to active texture?
  check();

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  check();

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glFlush();
  glFinish();
  check();

  eglSwapBuffers(state->display, state->surface);
  check();
}

void recompile_glsl(STATE* state) {
  state->should_recompile_glsl = 0;
  printf("GOT HERE\n");
  fflush(stdout);

  const GLchar *vertex_source_string =
      "attribute vec4 vertex;"
      "varying vec2 tcoord;"
      "void main(void) {"
      " vec4 pos = vertex;"
      " gl_Position = pos;"
      " tcoord = vertex.xy*0.5+0.5;"
      "}";

  // Mandelbrot
  const GLchar *fragment_source_mandelbrot =
      "uniform vec4 color;"
      "uniform vec2 scale;"
      "uniform vec2 centre;"
      "varying vec2 tcoord;"
      "void main(void) {"
      "  float intensity;"
      "  vec4 color2;"
      "  float cr=(gl_FragCoord.x-centre.x)*scale.x;"
      "  float ci=(gl_FragCoord.y-centre.y)*scale.y;"
      "  float ar=cr;"
      "  float ai=ci;"
      "  float tr,ti;"
      "  float col=0.0;"
      "  float p=0.0;"
      "  int i=0;"
      "  for(int i2=1;i2<16;i2++)"
      "  {"
      "    tr=ar*ar-ai*ai+cr;"
      "    ti=2.0*ar*ai+ci;"
      "    p=tr*tr+ti*ti;"
      "    ar=tr;"
      "    ai=ti;"
      "    if (p>16.0)"
      "    {"
      "      i=i2;"
      "      break;"
      "    }"
      "  }"
      "  color2 = vec4(float(i)*0.0625,0,0,1);"
      "  gl_FragColor = color2;"
      "}";

  // Julia
  const GLchar *fragment_source_julia =
      "uniform vec4 color;"
      "uniform vec2 scale;"
      "uniform vec2 centre;"
      "uniform vec2 offset;"
      "varying vec2 tcoord;"
      "uniform sampler2D tex;"
      "void main(void) {"
      "  float intensity;"
      "  vec4 color2;"
      "  float ar=(gl_FragCoord.x-centre.x)*scale.x;"
      "  float ai=(gl_FragCoord.y-centre.y)*scale.y;"
      "  float cr=(offset.x-centre.x)*scale.x;"
      "  float ci=(offset.y-centre.y)*scale.y;"
      "  float tr,ti;"
      "  float col=0.0;"
      "  float p=0.0;"
      "  int i=0;"
      "  vec2 t2;"
      "  t2.x=tcoord.x+(offset.x-centre.x)*(0.5/centre.y);"
      "  t2.y=tcoord.y+(offset.y-centre.y)*(0.5/centre.x);"
      "  for(int i2=1;i2<16;i2++)"
      "  {"
      "    tr=ar*ar-ai*ai+cr;"
      "    ti=2.0*ar*ai+ci;"
      "    p=tr*tr+ti*ti;"
      "    ar=tr;"
      "    ai=ti;"
      "    if (p>16.0)"
      "    {"
      "      i=i2;"
      "      break;"
      "    }"
      "  }"
      "  color2 = vec4(0,float(i)*0.0625,0,1);"
      "  color2 = color2+texture2D(tex,t2);"
      "  gl_FragColor = color2;"
      "}";

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  assert(vs != 0);
  glShaderSource(vs, 1, &vertex_source_string, 0);
  check();
  glCompileShader(vs);
  check();
  if (state->verbose) showlog(vs);

  GLuint js = glCreateShader(GL_FRAGMENT_SHADER);
  assert(js != 0);
  glShaderSource(js, 1, &fragment_source_julia, 0);
  check();
  glCompileShader(js);
  check();

  if (state->verbose) showlog(js);

  GLuint ms = glCreateShader(GL_FRAGMENT_SHADER);
  assert(ms != 0);
  glShaderSource(ms, 1, &fragment_source_mandelbrot, 0);
  check();
  glCompileShader(state->mshader);
  check();

  if (state->verbose) showlog(ms);


  // julia
  GLuint jp = glCreateProgram();
  assert(jp != 0);
  glAttachShader(jp, vs);
  check();
  glAttachShader(jp, js);
  check();
  glLinkProgram(jp);
  check();

  if (state->verbose) showprogramlog(jp);

/*

  state->attr_vertex = glGetAttribLocation(state->program_julia, "vertex");
  state->unif_color = glGetUniformLocation(state->program_julia, "color");
  state->unif_scale = glGetUniformLocation(state->program_julia, "scale");
  state->unif_offset = glGetUniformLocation(state->program_julia, "offset");
  state->unif_tex = glGetUniformLocation(state->program_julia, "tex");
  state->unif_centre = glGetUniformLocation(state->program_julia, "centre");

  // mandelbrot
  state->program_mandelbrot = glCreateProgram();
  glAttachShader(state->program_mandelbrot, state->vshader);
  glAttachShader(state->program_mandelbrot, state->mshader);
  glLinkProgram(state->program_mandelbrot);
  check();

  if (state->verbose) showprogramlog(state->program_mandelbrot);

  state->attr_vertex2 = glGetAttribLocation(state->program_mandelbrot, "vertex");
  state->unif_scale2 = glGetUniformLocation(state->program_mandelbrot, "scale");
  state->unif_offset2 = glGetUniformLocation(state->program_mandelbrot, "offset");
  state->unif_centre2 = glGetUniformLocation(state->program_mandelbrot, "centre");
  check();

  glClearColor(0.0, 1.0, 1.0, 1.0);

  glGenBuffers(1, &state->buf);

  check();

  // Prepare a texture image
  glGenTextures(1, &state->tex);
  check();
  glBindTexture(GL_TEXTURE_2D, state->tex);
  check();
  // glActiveTexture(0)
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, state->screen_width,
               state->screen_height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 0);
  check();
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  check();
  // Prepare a framebuffer for rendering
  glGenFramebuffers(1, &state->tex_fb);
  check();
  glBindFramebuffer(GL_FRAMEBUFFER, state->tex_fb);
  check();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         state->tex, 0);
  check();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  check();
  // Prepare viewport
  glViewport(0, 0, state->screen_width, state->screen_height);
  check();

  // Upload vertex data to a buffer
  glBindBuffer(GL_ARRAY_BUFFER, state->buf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data,
               GL_STATIC_DRAW);
  glVertexAttribPointer(state->attr_vertex, 4, GL_FLOAT, 0, 16, 0);
  glEnableVertexAttribArray(state->attr_vertex);
  glVertexAttribPointer(state->attr_vertex2, 4, GL_FLOAT, 0, 16, 0);
  glEnableVertexAttribArray(state->attr_vertex2);

  check();
*/

}

double tic() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return t.tv_sec + t.tv_nsec / 1000000000.0;
}

//==============================================================================
int generic_handler(const char *path, const char *types, lo_arg ** argv, int argc, lo_message data, void *user_data);
int xy_handler(const char *path, const char *types, lo_arg ** argv, int argc, lo_message data, void *user_data);
int frag_handler(const char *path, const char *types, lo_arg ** argv, int argc, lo_message data, void *user_data);

int main() {
  if (get_processor_id() == PROCESSOR_BCM2838) {
    puts("This demo application is not available on the Pi4\n\n");
    exit(0);
  }

  int terminate = 0;
  GLfloat cx, cy;
  bcm_host_init();


  // Clear application state
  STATE state;
  memset(&state, 0, sizeof(STATE));

  state.verbose = 1;

  lo_server_thread server;
  server = lo_server_thread_new("7770", NULL);
  lo_server_thread_add_method(server, "/frag", "s", frag_handler, &state);
  lo_server_thread_add_method(server, "/xy", "ii", xy_handler, &state);
  lo_server_thread_add_method(server, NULL, NULL, generic_handler, &state);
  lo_server_thread_start(server);

  // Start OGLES
  init_ogl(&state);
  init_shaders(&state);
  cx = state.screen_width / 2;
  cy = state.screen_height / 2;

  draw_mandelbrot_to_texture(&state, cx, cy, 0.003);

  double then = tic();
  while (!terminate) {

    if (state.should_recompile_glsl) {
      recompile_glsl(&state);
    }

    draw(&state, cx, cy, 0.003, state.x, state.y);

    // calculate FPS
    //
    double now = tic();
    double fps = 1 / (now - then);
    //printf("FPS: %.1lf delta: %.3lf\n", fps, 1 / fps);
    then = now;
  }

  lo_server_thread_free(server);
  return 0;
}

int generic_handler(const char *path, const char *types, lo_arg ** argv, int argc, lo_message data, void *user_data) {
    int i;

    printf("path: <%s>\n", path);
    for (i = 0; i < argc; i++) {
        printf("arg %d '%c' ", i, types[i]);
        lo_arg_pp(types[i], argv[i]);
        printf("\n");
    }
    printf("\n");
    fflush(stdout);

    return 0;
}

int xy_handler(const char *path, const char *types, lo_arg ** argv, int argc, lo_message data, void *user_data) {
    ((STATE*)user_data)->x = argv[0]->i;
    ((STATE*)user_data)->y = argv[1]->i;
    //printf("x:%d y:%d\n", argv[0]->i, argv[1]->i);
    //fflush(stdout);
    return 0;
}

int frag_handler(const char *path, const char *types, lo_arg ** argv, int argc, lo_message data, void *user_data) {
    strncpy(((STATE*)user_data)->glsl, (char*)argv[0], GLSL_SIZE);
    ((STATE*)user_data)->should_recompile_glsl = 1;
    printf("frag: %s\n", (char*)argv[0]);
    fflush(stdout);
    return 0;
}
