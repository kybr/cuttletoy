// #define NDEBUG

#include <cassert>
// <cassert> must come before liblo on the pi

#include <curses.h>
#include <lo/lo.h>
#include <lo/lo_cpp.h>

int main(int argc, char* argv[]) {
  bool disable_curses = argc > 1;

  setlocale(LC_ALL, "");
  //setlocale(LC_ALL, "en_US.UTF-8");
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

  move(10, 10);
  printw("COLS:%d LINES:%d", COLS, LINES);
  refresh();

  if (disable_curses) {
    endwin();
  }

  lo::Address client("224.0.7.24", "7771");

  client.send("/size", "ii", COLS, LINES);

  lo::ServerThread server("224.0.7.23", "7770", nullptr, nullptr,
                          [](int n, const char* message, const char* where) {
                            assert(false);
                            fflush(stdout);
                          });
  assert(server.is_valid());

  server.add_method("/print", "iis",
                    [&](lo_arg** argv, int argc, lo::Message m) {
                      int x = argv[0]->i;
                      int y = argv[1]->i;
                      if ((x < 0) || (x >= COLS) || (y < 0) || (y >= LINES)) {
                        return;
                      }
		      if (disable_curses) {
		        printf("%s\n", &argv[2]->s);
			return;
		      }
                      move(y, x);  // it is move(y, x) aka move(line, column)
                      printw("%s", &argv[2]->s);
                      refresh();
                    });

  server.add_method("/bkgd", "i", [&](lo_arg** argv, int argc, lo::Message m) {
    bkgd(COLOR_PAIR(argv[0]->i));
    refresh();
  });

  server.add_method("/quit", nullptr,
                    [&](lo_arg** argv, int argc, lo::Message m) { exit(0); });

  server.start();

  getchar();

  endwin();
}
