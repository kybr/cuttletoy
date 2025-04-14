
#include <cassert>
// <cassert> must come before liblo on the pi

#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include <chrono>
#include <cstring>
#include <iostream>

#include "GL.h"
#include "Help.hpp"
#include "Window.hpp"
#include "Conf.hpp"

static const struct {
  float x, y;
} vertices[4] = {{-1.0, -1.0}, {-1.0, 1.0}, {1.0, 1.0}, {1.0, -1.0}};
class Toy {
  GLuint program;
  GLuint vertex_shader;
  GLuint fragment_shader;
  GLint location_attribute_position;
  GLuint vertex_buffer;
  GLint location_time;
  GLint location_size;

 public:
  Toy() {
    auto vertex_string = slurp("vertex.glsl");
    const char* vertex_shader_text = vertex_string.c_str();

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    assert(vertex_shader);

    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    assert(gl_good());
    glCompileShader(vertex_shader);
    assert(gl_good());
    {
      std::string error;
      if (!check_shader_compile(vertex_shader, error)) {
        fprintf(stderr, "Error compiling GLSL:\n%s\n", error.c_str());
        exit(1);
      }
    }

    auto fragment_string = slurp("fragment.glsl");
    const char* fragment_shader_text = fragment_string.c_str();

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    assert(fragment_shader);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    assert(gl_good());
    glCompileShader(fragment_shader);
    assert(gl_good());

    {
      std::string error;
      if (!check_shader_compile(fragment_shader, error)) {
        fprintf(stderr, "Error compiling GLSL:\n%s\n", error.c_str());
        exit(1);
      }
    }

    program = glCreateProgram();
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
        exit(1);
      }
    }

    location_attribute_position = glGetAttribLocation(program, "position");
    assert(location_attribute_position != -1);
    glEnableVertexAttribArray(location_attribute_position);
    assert(gl_good());

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    assert(gl_good());

    glVertexAttribPointer(location_attribute_position, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*)0);
    assert(gl_good());

    location_time = glGetUniformLocation(program, "time");
    location_size = glGetUniformLocation(program, "size");
    assert(gl_good());
  }

  void draw(int width, int height, double time) {
    glViewport(0, 0, width, height);
    assert(gl_good());
    glClear(GL_COLOR_BUFFER_BIT);

    assert(gl_good());
    glUseProgram(program);
    assert(gl_good());
    glUniform1f(location_time, time);
    assert(gl_good());
    glUniform2f(location_size, width, height);
    assert(gl_good());

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    assert(gl_good());
  }

  bool compile(const char* fragment_shader_text, std::string& error) {
    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    glCompileShader(fragment_shader);
    if (!check_shader_compile(fragment_shader, error)) {
      fprintf(stderr, "Error compiling GLSL:\n%s\n", error.c_str());
      return false;
    }

    // XXX maybe delete program?

    auto program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    if (!check_shader_link(program, error)) {
      fprintf(stderr, "Error linking program:\n%s\n", error.c_str());
      return false;
    }

    auto location_attribute_position = glGetAttribLocation(program, "position");
    if (location_attribute_position == -1) {
      // XXX this should never happen
      exit(1);
      error += "Could not find 'position' attribute\n";
      fprintf(stderr, "Error: no position attribute\n");
      return false;
    }
    glEnableVertexAttribArray(location_attribute_position);
    glVertexAttribPointer(location_attribute_position, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*)0);

    auto location_time = glGetUniformLocation(program, "time");
    auto location_size = glGetUniformLocation(program, "size");

    if (gl_good()) {
      // this new shader is good; overwrite the old stuff
      this->program = program;
      this->fragment_shader = fragment_shader;
      this->location_attribute_position = location_attribute_position;
      this->location_time = location_time;
      this->location_size = location_size;
      return true;
    }

    error += "Unknown error\n";
    return false;
  }
};

int main(int argc, char* argv[]) {
  auto begining = std::chrono::steady_clock::now();

  Conf conf;
  if (!conf.load()) {
    printf("No configuration loaded\n");
  }

  // if we are on the pi, load a config file that
  // determines uniforms and such
  {
    std::string mac;
    std::ifstream file("/sys/class/net/eth0/address");
    if (file.is_open()) {
      if (std::getline(file, mac)) {
        printf("got mac address %s\n", mac.c_str());
      }
    }
  }

  lo::ServerThread server(
      7770, [](int n, const char* message, const char* where) {
        std::cout << "ERROR: " << message << "(" << where << ")" << std::endl;
        fflush(stdout);
      });
  assert(server.is_valid());

  bool hasNewFrag = false;
  char fragment[65000];

  lo::Address* remote = nullptr;
  server.add_method("/frag", "s", [&](lo_arg** argv, int, lo::Message m) {
    if (remote) {
      delete remote;
    }
    remote = new lo::Address(m.source().hostname(), 7771);

    strncpy(fragment, &argv[0]->s, sizeof(fragment));
    hasNewFrag = true;

    printf("fragment is %d bytes\n", (int)strlen(fragment));
    fflush(stdout);
  });

  server.start();

  Window window;
  Toy toy;

  auto then = std::chrono::steady_clock::now();
  while (!window.done()) {
    auto now = std::chrono::steady_clock::now();

    if (hasNewFrag) {
      hasNewFrag = false;
      std::string error;
      if (!toy.compile(fragment, error)) {
        if (remote) {
          remote->send("/err", "s", error.c_str());
        }
      }
    }

    int width, height;
    window.size(width, height);

    double time = std::chrono::duration<double>(now - begining).count();
    toy.draw(width, height, time);
    window.swap();

    double dt = std::chrono::duration<double>(now - then).count();
    then = now;
    double fps = 1 / dt;
    // printf("FPS: %.1lf delta: %.3lf\n", fps, dt);
  }
}
