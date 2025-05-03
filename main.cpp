// #define NDEBUG

#include <cassert>
// <cassert> must come before liblo on the pi

#include <curses.h>
#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>

#include "Toy.hpp"

int main(int argc, char* argv[]) {
  double time = 0.0;
  setlocale(LC_ALL, "");
  auto begining = std::chrono::steady_clock::now();

  initscr();
  cbreak();
  noecho();
  curs_set(0);
  std::atexit([]() { endwin(); });
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

  bkgd(COLOR_PAIR(1));

  // move(10, 10);
  // printw("COLS:%d LINES:%d", COLS, LINES);

  Toy toy;

  // move(15, 0);
  // printw("pi: %d column:%d row:%d", toy.conf.id, toy.conf.x_screen,
  // toy.conf.y_screen);

  lo::Address client("224.0.7.24", "7771");

  client.send("/size", "ii", COLS, LINES);

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

    // move(2, 0);
    // printw("fragment is %d bytes\n", (int)strlen(fragment));
    // refresh();
  });

  server.add_method("/time", "d", [&](lo_arg** argv, int, lo::Message m) {
    time = argv[0]->d;
  });

  server.add_method("/print", "iis",
                    [&](lo_arg** argv, int argc, lo::Message m) {
                      int x = argv[0]->i - toy.conf.x_screen * COLS;
                      int y = argv[1]->i - toy.conf.y_screen * LINES;
                      if ((x < 0) || (x >= COLS) || (y < 0) || (y >= LINES)) {
                        // endwin();
                        // printf("got %d %d\n", y, x);
                        // fflush(stdout);
                        // exit(1);
                        return;
                      }
                      move(y, x);  // it is move(y, x) aka move(line, column)
                      printw("%s", &argv[2]->s);
                      refresh();
                    });

  server.add_method("/logout", "", [&](lo_arg** argv, int argc, lo::Message m) {
    system("pkill -t tty1 bash -9");
  });

  server.add_method("/display", "i",
                    [&](lo_arg** argv, int argc, lo::Message m) {
                      if (argv[0]->i == 0) {
                        system("vcgencmd display_power 0");
                      } else {
                        system("vcgencmd display_power 1");
                      }
                    });

  server.add_method("/bkgd", "i", [&](lo_arg** argv, int argc, lo::Message m) {
    bkgd(COLOR_PAIR(argv[0]->i));
    refresh();
  });

  server.add_method("/quit", nullptr,
                    [&](lo_arg** argv, int argc, lo::Message m) { exit(0); });

  server.start();

  int framecount = 0;
  bool running = true;

  std::thread sendfps([&]() {
    lo::Address them("localhost", 7777);
    while (running) {
      them.send("/fps", "i", framecount);

      // curses output....
      // move(0, 0);
      // printw("FPS: %d", framecount);
      // refresh();

      framecount = 0;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  });

  auto then = std::chrono::steady_clock::now();
  while (!toy.done()) {
    if (hasNewFrag) {
      hasNewFrag = false;
      std::string error;
      auto tic = std::chrono::steady_clock::now();
      bool success = toy.compile(fragment, error);
      float t =
          std::chrono::duration<double>(std::chrono::steady_clock::now() - tic)
              .count();
      if (success) {
        if (remote) {
          char buffer[10000];
          int ms = t * 1000;
          snprintf(buffer, sizeof(buffer), "compiled in %d millisecond%s", ms, ms == 1 ? "" : "s");
          remote->send("/err", "s", buffer);
        }
      } else {
        if (remote) {
          remote->send("/err", "s", error.c_str());
        }
      }
    }

    auto now = std::chrono::steady_clock::now();
    double dt = std::chrono::duration<double>(now - then).count();
    then = now;
    time += dt;
    toy.draw(time);

    // move(5, 0);
    // printw("time: %.3lf", time);
    // refresh(); // does this block? will it make things slow?
    framecount++;
  }

  running = false;
  sendfps.join();

  endwin();
}
