// #define NDEBUG

#include <cassert>
// <cassert> must come before liblo on the pi

#include <lo/lo.h>

#include "lo_cpp_modified.h"
// #include <lo/lo_cpp.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

#include "Toy.hpp"

int main(int argc, char* argv[]) {
  auto begining = std::chrono::steady_clock::now();

  Toy toy;

  lo::ServerThread server(
      "224.0.7.23", "7770", [](int n, const char* message, const char* where) {
        std::cout << "ERROR: " << message << "(" << where << ")" << std::endl;
        fflush(stdout);
      });
  assert(server.is_valid());

  server.add_method(
      "/button/south", "i",
      [&](lo_arg** argv, int, lo::Message m) { toy.u_button.x = argv[0]->i; });
  server.add_method(
      "/button/east", "i",
      [&](lo_arg** argv, int, lo::Message m) { toy.u_button.y = argv[0]->i; });
  server.add_method(
      "/button/north", "i",
      [&](lo_arg** argv, int, lo::Message m) { toy.u_button.z = argv[0]->i; });
  server.add_method(
      "/button/west", "i",
      [&](lo_arg** argv, int, lo::Message m) { toy.u_button.w = argv[0]->i; });
  server.add_method("/hat/x", "i", [&](lo_arg** argv, int, lo::Message m) {
    toy.u_hat.x = argv[0]->i;
  });
  server.add_method("/hat/y", "i", [&](lo_arg** argv, int, lo::Message m) {
    toy.u_hat.y = argv[0]->i;
  });
  server.add_method("/stick/x", "f", [&](lo_arg** argv, int, lo::Message m) {
    toy.u_analog_left.x = argv[0]->f;
  });
  server.add_method("/stick/y", "f", [&](lo_arg** argv, int, lo::Message m) {
    toy.u_analog_left.y = argv[0]->f;
  });
  server.add_method("/stick/rx", "f", [&](lo_arg** argv, int, lo::Message m) {
    toy.u_analog_right.x = argv[0]->f;
  });
  server.add_method("/stick/ry", "f", [&](lo_arg** argv, int, lo::Message m) {
    toy.u_analog_right.y = argv[0]->f;
  });
  server.add_method("/trigger/z", "f", [&](lo_arg** argv, int, lo::Message m) {
    toy.u_analog_left.z = argv[0]->f;
  });
  server.add_method("/trigger/rz", "f", [&](lo_arg** argv, int, lo::Message m) {
    toy.u_analog_right.z = argv[0]->f;
  });

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

  while (!toy.done()) {
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

    double time = std::chrono::duration<double>(now - begining).count();

    toy.draw(time);

    double dt = std::chrono::duration<double>(now - then).count();
    then = now;
    double fps = 1 / dt;

    framecount++;
  }

  running = false;
  sendfps.join();
}
