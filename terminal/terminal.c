#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  initscr();
  cbreak();
  noecho();
  curs_set(0);

  while (1) {
    //clear();
    int yi = random() % LINES;
    int xi = random() % COLS;
    char c = random() & 255;
    if (c < 32 || c == 127 || c == 255) {
      continue;
    }
    mvaddch(yi, xi, c);
    refresh();
    usleep(10000);
  }

  getch();
  endwin();
  exit(0);
}
