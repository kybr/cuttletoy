// #define NDEBUG

#include <cassert>
// <cassert> must come before liblo on the pi

#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

#include "Toy.hpp"

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
