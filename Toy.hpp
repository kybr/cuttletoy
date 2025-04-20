#pragma once
#include <cassert>
#include <string>

#include "Conf.hpp"
#include "GL.h"
#include "Help.hpp"
#include "Toy.hpp"
#include "Window.hpp"

static const struct {
  float x, y;
} vertices[4] = {{-1.0, -1.0}, {-1.0, 1.0}, {1.0, 1.0}, {1.0, -1.0}};

struct vec2 {
  float x, y;
};
struct vec3 {
  float x, y, z;
};
struct vec4 {
  float x, y, z, w;
};

class Toy {
  Window window;
  GLuint program;
  GLuint vertex_shader;
  GLint location_attribute_position;
  GLuint vertex_buffer;

  // uniforms.....
  GLint uniform_time;
  GLint uniform_size;
  GLint uniform_screen;
  GLint uniform_analog_left;
  GLint uniform_analog_right;
  GLint uniform_hat;
  GLint uniform_button;
  GLint uniform_random;

  // float u_time; // xy is which screen; z is id, where -1 means not a screen
  // vec3 u_screen; // xy is which screen; z is id (-1, 15) where -1 means not a
  // pi
  // vec3 u_analog_left;  // xy is joystick; z is trigger vec3 u_analog_right;
  // vec2 u_hat; // x and y are (-1, 0, 1)
  // vec4 u_button; // x, y, z, w are buttons (0, 1)
  // vec4 u_random; // random values

  Conf conf;

 public:
  float u_time = 0.0f;
  vec3 u_screen = {0.0f, 0.0f, -1.0f};
  vec3 u_analog_left = {0.0f, 0.0f, 0.0f};
  vec3 u_analog_right = {0.0f, 0.0f, 0.0f};
  vec2 u_hat = {0.0f, 0.0f};
  vec4 u_button = {0.0f, 0.0f, 0.0f, 0.0f};
  vec4 u_random = {0.0f, 0.0f, 0.0f, 0.0f};

  Toy() {
    if (conf.load()) {
      printf("toy.conf loaded\n");
    }
    conf.show();

    // these never change
    u_screen.x = conf.x_screen;
    u_screen.y = conf.y_screen;
    u_screen.z = conf.id;

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
    uniform_screen = glGetUniformLocation(program, "u_screen");
    uniform_analog_left = glGetUniformLocation(program, "u_analog_left");
    uniform_analog_right = glGetUniformLocation(program, "u_analog_right");
    uniform_hat = glGetUniformLocation(program, "u_hat");
    uniform_button = glGetUniformLocation(program, "u_button");
    uniform_random = glGetUniformLocation(program, "u_random");

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
    auto uniform_screen = glGetUniformLocation(program, "u_screen");
    auto uniform_analog_left = glGetUniformLocation(program, "u_analog_left");
    auto uniform_analog_right = glGetUniformLocation(program, "u_analog_right");
    auto uniform_hat = glGetUniformLocation(program, "u_hat");
    auto uniform_button = glGetUniformLocation(program, "u_button");
    auto uniform_random = glGetUniformLocation(program, "u_random");

    if (gl_good()) {
      // this new shader is good; overwrite the old stuff
      glDeleteProgram(this->program);
      glUseProgram(program);
      this->program = program;
      this->uniform_time = uniform_time;
      this->uniform_size = uniform_size;
      this->uniform_screen = uniform_screen;
      this->uniform_analog_left = uniform_analog_left;
      this->uniform_analog_right = uniform_analog_right;
      this->uniform_hat = uniform_hat;
      this->uniform_button = uniform_button;
      this->uniform_random = uniform_random;
      return true;
    }

    error += "Unknown error\n";
    return false;
  }

  void draw(double time) {
    int width, height;
    window.size(width, height);

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

    glUniform3f(uniform_screen, u_screen.x, u_screen.y, u_screen.z);
    glUniform3f(uniform_analog_left, u_analog_left.x, u_analog_left.y,
                u_analog_left.z);
    glUniform3f(uniform_analog_right, u_analog_right.x, u_analog_right.y,
                u_analog_right.z);
    glUniform2f(uniform_hat, u_hat.x, u_hat.y);
    glUniform4f(uniform_button, u_button.x, u_button.y, u_button.z, u_button.w);
    glUniform4f(uniform_random, u_random.x, u_random.y, u_random.z, u_random.w);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    assert(gl_good());

    window.swap();
  }

  bool done() { return window.done(); }
};
