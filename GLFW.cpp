#include "Window.hpp"
#include <cstdlib> // exit

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct WindowImp {
  GLFWwindow* window;

  WindowImp() {
    if (!glfwInit()) {
      exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    window = glfwCreateWindow(640, 480, "Toy", NULL, NULL);
    if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }

    //glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
  }

  ~WindowImp() {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void swap() {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  bool done() {
    return glfwWindowShouldClose(window);
  }

  void size(int& width, int& height) {
    glfwGetFramebufferSize(window, &width, &height);
  }
};

Window::Window() { i = new WindowImp(); }
Window::~Window() { delete i; }
void Window::swap() { i->swap(); }
bool Window::done() { return i->done(); }
void Window::size(int& width, int& height) { i->size(width, height); }
