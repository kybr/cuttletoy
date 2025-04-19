// #define NDEBUG

#include <cassert>
// <cassert> must come before liblo on the pi

#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

#include "Conf.hpp"
#include "GL.h"
#include "Help.hpp"
#include "Window.hpp"

static const struct {
  float x, y;
} vertices[4] = {{-1.0, -1.0}, {-1.0, 1.0}, {1.0, 1.0}, {1.0, -1.0}};

class Toy {
  GLuint program;
  GLuint vertex_shader;
  GLint location_attribute_position;
  GLuint vertex_buffer;

  // uniforms.....
  GLint uniform_time;
  GLint uniform_size;
  GLint uniform_screen;
  GLint uniform_screen_maximum;
  GLint uniform_pixel_offset;
  GLint uniform_pixel_offset_maximum;
  GLint uniform_parameter;

  Conf conf;
  bool running_on_pi = false;

 public:
  void load_conf_uniforms() {}
  void send_conf_uniforms() {}

  Toy() {
    if (conf.load()) {
      conf.show();
      running_on_pi = true;
    }

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

    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
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

    glDeleteShader(fragment_shader);

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

    uniform_time = glGetUniformLocation(program, "u_time");
    uniform_size = glGetUniformLocation(program, "u_size");
    assert(gl_good());

    glUseProgram(program);
  }

  bool compile(const char* fragment_shader_text, std::string& error) {
    auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    glCompileShader(fragment_shader);
    if (!check_shader_compile(fragment_shader, error)) {
      fprintf(stderr, "Error compiling GLSL:\n%s\n", error.c_str());
      return false;
    }

    auto program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    if (!check_shader_link(program, error)) {
      fprintf(stderr, "Error linking program:\n%s\n", error.c_str());
      return false;
    }

    glDeleteShader(fragment_shader);

    auto uniform_time = glGetUniformLocation(program, "u_time");
    auto uniform_size = glGetUniformLocation(program, "u_size");

    if (gl_good()) {
      // this new shader is good; overwrite the old stuff
      glDeleteProgram(this->program);
      glUseProgram(program);
      this->program = program;
      this->uniform_time = uniform_time;
      this->uniform_size = uniform_size;
      return true;
    }

    error += "Unknown error\n";
    return false;
  }

  void draw(int width, int height, double time) {
    glViewport(0, 0, width, height);
    assert(gl_good());
    glClear(GL_COLOR_BUFFER_BIT);
    assert(gl_good());

    // which uniforms must be set on every draw call?
    // - time
    // - width and height (on the desktop)

    glUniform1f(uniform_time, time);
    assert(gl_good());
    glUniform2f(uniform_size, width, height);
    assert(gl_good());

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    assert(gl_good());
  }
};

int main(int argc, char* argv[]) {
  auto begining = std::chrono::steady_clock::now();

  lo::ServerThread server(
      7770, [](int n, const char* message, const char* where) {
        std::cout << "ERROR: " << message << "(" << where << ")" << std::endl;
        fflush(stdout);
      });
  assert(server.is_valid());

  bool hasNewFrag = false;
  char fragment[65000];

  lo::Address* remote = nullptr;
  server.add_method("/f", "s", [&](lo_arg** argv, int, lo::Message m) {
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

  int framecount = 0;

  bool running = true;

  std::thread sendfps([&]() {
    lo::Address them("localhost", 7777);
    while (running) {
      them.send("/fps", "i", framecount);
      printf("FPS: %d\n", framecount);
      fflush(stdout);
      framecount = 0;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  });

  while (!window.done()) {
    auto now = std::chrono::steady_clock::now();

    if (hasNewFrag) {
      hasNewFrag = false;
      std::string error;
      auto tic = std::chrono::steady_clock::now();
      if (!toy.compile(fragment, error)) {
        if (remote) {
          remote->send("/err", "s", error.c_str());
        }
      }
      printf(
          "fragment compile took %.3lf ms\n",
          std::chrono::duration<double>(std::chrono::steady_clock::now() - tic)
                  .count() *
              1000);
    }

    int width, height;
    window.size(width, height);

    double time = std::chrono::duration<double>(now - begining).count();
    toy.draw(width, height, time);
    window.swap();

    double dt = std::chrono::duration<double>(now - then).count();
    then = now;
    double fps = 1 / dt;

    framecount++;
  }

  running = false;
  sendfps.join();
}
