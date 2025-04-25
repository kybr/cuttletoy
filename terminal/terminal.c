#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

void handle() { endwin(); }

int main(void) {
  signal(SIGTERM, handle);

  initscr();
  cbreak();
  noecho();
  curs_set(0);

  if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

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

  bkgd(COLOR_PAIR(15));

  // move(x, y);
  // printw("...");

  while (1) {
    //clear();
    int yi = random() % LINES;
    int xi = random() % COLS;
    unsigned char c = random() & 255;
    if (c < 32 || c == 127 || c == 255) {
      continue;
    }
    attron(COLOR_PAIR(15 + random() % 7));
    mvaddch(yi, xi, c);
    refresh();
    usleep(10000);
  }
}
