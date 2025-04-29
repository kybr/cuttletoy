#include <lo/lo.h>
#include <math.h>  // sqrt, log
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "data/h/Unifont-APL8x16-16.0.03.psf.gz.txt.h"
// #include "data/h/Uni3-Terminus24x12.psf.gz.txt.h"

int state = 0;
float rndf() {
  state += 12345;
  state *= 1103515245;  // overflow
  return state / 4294967296.0;
}
unsigned rndu() {
  state += 12345;
  state *= 1103515245;  // overflow
  return state;
}

double normal() {
  double u = ((double)rand() / (RAND_MAX)) * 2 - 1;
  double v = ((double)rand() / (RAND_MAX)) * 2 - 1;
  double r = u * u + v * v;
  if (r == 0 || r > 1) return normal();
  double c = sqrt(-2 * log(r) / r);
  return u * c;
}

const int length = sizeof(glyph) / sizeof(glyph[0]);

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Provide terminal width and height\n");
    exit(1);
  }

  int COLS = atoi(argv[1]);
  int LINES = atoi(argv[2]);
  if (COLS <= 10 || LINES <= 10) {
    fprintf(stderr, "Are you kidding?\n");
    exit(1);
  }

  printf("%d x %d\n", COLS, LINES);

  srand(0);

  lo_address t = lo_address_new("224.0.7.23", "7770");
  while (1) {
    usleep(25000);
    int x = rand() % COLS;
    int y = rand() % LINES;
    int i = rand() % length;
    printf("%d: %s\n", i, glyph[i]);
    if (lo_send(t, "/print", "iis", x, y, glyph[i]) == -1) {
      printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
    }
  }
}
