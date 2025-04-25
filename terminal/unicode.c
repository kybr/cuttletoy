//#define _X_OPEN_SOURCE_EXTENDED // maybe not necessary
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <wchar.h>

int main(int argc, char **argv) {
  setlocale(LC_ALL, ""); // necessary...
  initscr();
  cbreak();
  refresh();
  mvaddch(0, 0, 'a');
  mvaddstr(1, 0, "\u2560");
  refresh();
  getchar();
  endwin();
  return 0;
}
