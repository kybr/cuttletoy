// #define NDEBUG

#include <cassert>
// <cassert> must come before liblo on the pi

#include <curses.h>
#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

#include "Toy.hpp"



int main(int argc, char* argv[]) {
  auto begining = std::chrono::steady_clock::now();

  initscr();
  cbreak();
  noecho();
  curs_set(0);
  std::atexit([]() {
    endwin();
  });
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_BLUE, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);

  init_pair(8, COLOR_BLACK, COLOR_RED);
  init_pair(9, COLOR_BLACK, COLOR_GREEN);
  init_pair(10, COLOR_BLACK, COLOR_YELLOW);
  init_pair(11, COLOR_BLACK, COLOR_BLUE);
  init_pair(12, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(13, COLOR_BLACK, COLOR_CYAN);
  init_pair(14, COLOR_BLACK, COLOR_WHITE);

  init_pair(15, COLOR_WHITE, COLOR_WHITE);
  init_pair(16, COLOR_RED, COLOR_WHITE);
  init_pair(17, COLOR_GREEN, COLOR_WHITE);
  init_pair(18, COLOR_YELLOW, COLOR_WHITE);
  init_pair(19, COLOR_BLUE, COLOR_WHITE);
  init_pair(20, COLOR_MAGENTA, COLOR_WHITE);
  init_pair(21, COLOR_CYAN, COLOR_WHITE);

  move(10, 10);
  printw("COLS:%d LINES:%d", COLS, LINES);

  Toy toy;

  lo::ServerThread server("224.0.7.23", "7770", nullptr, nullptr,
                          [](int n, const char* message, const char* where) {
                            std::cout << "ERROR: " << message << "(" << where
                                      << ")" << std::endl;
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

    move(2, 0);
    printw("fragment is %d bytes\n", (int)strlen(fragment));
    refresh();
  });

  server.add_method("/print", "iis", [&](lo_arg** argv, int argc, lo::Message m) {
    if (argc != 3) {
      return;
    }
    move(argv[0]->i, argv[1]->i);
    printw("%s", &argv[2]->s);
    refresh();
  });

  server.add_method("/bkgd", "i", [&](lo_arg** argv, int argc, lo::Message m) {
    if (argc != 1) {
      return;
    }
    bkgd(COLOR_PAIR(argv[0]->i));
    //refresh();
  });
  server.add_method("/quit", "", [&](lo_arg** argv, int argc, lo::Message m) {
    exit(0);
  });


  server.start();

  auto then = std::chrono::steady_clock::now();

  int framecount = 0;
  bool running = true;

  std::thread sendfps([&]() {
    lo::Address them("localhost", 7777);
    while (running) {
      them.send("/fps", "i", framecount);

      // curses output....
      move(0, 0);
      printw("FPS: %d", framecount);
      refresh();

      framecount = 0;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  });

  while (!toy.done()) {

    if (hasNewFrag) {
      hasNewFrag = false;
      std::string error;
      auto tic = std::chrono::steady_clock::now();
      if (!toy.compile(fragment, error)) {
        if (remote) {
          remote->send("/err", "s", error.c_str());
        }
      }
      move(1, 0);
      printw(
          "fragment compile took %.3lf ms\n",
          std::chrono::duration<double>(std::chrono::steady_clock::now() - tic)
                  .count() *
              1000);
      refresh();
    }

    auto now = std::chrono::steady_clock::now();
    double time = std::chrono::duration<double>(now - begining).count();
    toy.draw(time);

    //double dt = std::chrono::duration<double>(now - then).count();
    //then = now;

    framecount++;
  }

  running = false;
  sendfps.join();

  endwin();
}
