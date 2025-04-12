#pragma once

// choose the right OpenGL header...
//
#ifdef __APPLE__
#include <OpenGL/gl.h>
#elif __RPI__
#include "GLES2/gl2.h"
#endif

#include <iostream>

bool gl_good() {
  switch (glGetError()) {
    case GL_NO_ERROR:
      return true;
    case GL_INVALID_ENUM:
      fprintf(stderr, "GL_INVALID_ENUM\n");
      break;
    case GL_INVALID_VALUE:
      fprintf(stderr, "GL_INVALID_VALUE\n");
      break;
    case GL_INVALID_OPERATION:
      fprintf(stderr, "GL_INVALID_OPERATION\n");
      break;
    case GL_STACK_OVERFLOW:
      fprintf(stderr, "GL_STACK_OVERFLOW\n");
      break;
    case GL_STACK_UNDERFLOW:
      fprintf(stderr, "GL_STACK_UNDERFLOW\n");
      break;
    case GL_OUT_OF_MEMORY:
      fprintf(stderr, "GL_OUT_OF_MEMORY\n");
      break;
    default:
      fprintf(stderr, "Unknown error\n");
  }
  return false;
}

bool check_shader_compile(GLuint shader, std::string& error) {
  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (compiled != GL_TRUE) {
    GLsizei log_length = 0;
    GLchar message[1024];
    glGetShaderInfoLog(shader, 1024, &log_length, message);
    error += message;
  }
  return compiled == GL_TRUE;
}