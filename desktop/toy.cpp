#include <cassert>
#include <fstream>

#include "GL.h"
#include "Window.hpp"
std::string slurp(std::string fileName) {
  std::fstream file(fileName);
  std::string returnValue = "";
  while (file.good()) {
    std::string line;
    getline(file, line);
    returnValue += line + "\n";
  }
  return returnValue;
}

/*
class ShaderManager {
  std::unordered_map<std::string, GLint> uniform;
  public:

  // tries to compile and link
  bool submit(const char* fragment) {
  }
};
*/

#include <chrono>

int main(int argc, char* argv[]) {
  std::chrono::high_resolution_clock::time_point begining =
      std::chrono::high_resolution_clock::now();

  Window window;

  auto vertex_string = slurp("vertex.glsl");
  const char* vertex_shader_text = vertex_string.c_str();

  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  assert(vertex_shader);

  glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
  assert(gl_good());
  glCompileShader(vertex_shader);
  assert(gl_good());
  {
    std::string error;
    if (!check_shader_compile(vertex_shader, error)) {
      fprintf(stderr, "Error compiling GLSL:\n%s\n", error.c_str());
      return 1;
    }
  }

  auto fragment_string = slurp("fragment.glsl");
  const char* fragment_shader_text = fragment_string.c_str();

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  assert(fragment_shader);
  glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
  assert(gl_good());
  glCompileShader(fragment_shader);
  assert(gl_good());

  {
    std::string error;
    if (!check_shader_compile(fragment_shader, error)) {
      fprintf(stderr, "Error compiling GLSL:\n%s\n", error.c_str());
      return 1;
    }
  }

  GLuint program = glCreateProgram();
  assert(program);
  glAttachShader(program, vertex_shader);
  assert(gl_good());
  glAttachShader(program, fragment_shader);
  assert(gl_good());
  glLinkProgram(program);
  assert(gl_good());

  {
    std::string error;
    if (!check_shader_link(program, error)) {
      fprintf(stderr, "Error linking program:\n%s\n", error.c_str());
      return 1;
    }
  }

  GLint vpos_location = glGetAttribLocation(program, "position");
  assert(vpos_location != -1);
  glEnableVertexAttribArray(vpos_location);
  assert(gl_good());

  static const struct {
    float x, y;
  } vertices[4] = {{-1.0, -1.0}, {-1.0, 1.0}, {1.0, 1.0}, {1.0, -1.0}};

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  assert(gl_good());

  glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                        sizeof(vertices[0]), (void*)0);
  assert(gl_good());

  GLint location_time = glGetUniformLocation(program, "time");
  GLint location_size = glGetUniformLocation(program, "size");

  auto then = std::chrono::high_resolution_clock::now();
  while (!window.done()) {
    int width, height;
    window.size(width, height);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    auto now = std::chrono::high_resolution_clock::now();
    double t = std::chrono::duration<double>(now - begining).count();
    double dt = std::chrono::duration<double>(now - then).count();
    then = now;

    glUseProgram(program);
    glUniform1f(location_time, t);
    glUniform2f(location_size, width, height);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    double fps = 1 / dt;
    printf("FPS: %.1lf delta: %.3lf\n", fps, dt);

    window.swap();
  }
}
