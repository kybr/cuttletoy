#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define N (100)
double delta[N];
int i = 0;


int main(void) {
  struct timespec tv;
  clock_gettime(CLOCK_MONOTONIC, &tv);
  double then = tv.tv_sec + tv.tv_nsec / 1000000000.0;

  initscr();
  cbreak();
  noecho();
  curs_set(0);

  while (1) {
    clear();
    for (int iter = 0; iter < 100; iter++) {
      int yi = random() % LINES;
      int xi = random() % COLS;
      mvaddch(yi, xi, (char)(random() & 127));
    }
    refresh();
    clock_gettime(CLOCK_MONOTONIC, &tv);
    double now = tv.tv_sec + tv.tv_nsec / 1000000000.0;
    double dt = now - then;
    then = now;
    delta[i] = dt;
    i++;
    if (i == N) {
      break;
    }
  }

  FILE *file_pointer;
  char *file_name = "log.txt";
  file_pointer = fopen(file_name, "w");
  for (int i = 0; i < N; ++i) {
    fprintf(file_pointer, "%lf\n", delta[i]);
  }
  fclose(file_pointer);

  getch();
  endwin();
  exit(0);
}
