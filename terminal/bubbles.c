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

#define NBUBLE (50)
#define WAIT (11000)

typedef struct {
    int x, y;
    int state;
} Bubble;
Bubble bubble[NBUBLE];

int main(int argc, char* argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Provide terminal width and height and runtime\n");
    exit(1);
  }

  int COLS = atoi(argv[1]);
  int LINES = atoi(argv[2]);
  double LENGTH = atof(argv[3]);
  if (COLS <= 10 || LINES <= 10) {
    fprintf(stderr, "Are you kidding?\n");
    exit(1);
  }

  srand(0);

  printf("%d x %d\n", COLS, LINES);

  for (int i = 0; i < NBUBLE; i++) {
      bubble[i].x = rndu() % COLS;
      bubble[i].y = rndu() % LINES;
      bubble[i].state = rndu() % 2;
  }

  lo_address t = lo_address_new("224.0.7.23", "7770");
  double runtime = 0;
  while (1) {

    for (int k = 0; k < NBUBLE; ++k) {
        usleep(WAIT);

        int i = rand() % length;

        if (lo_send(t, "/print", "iis", bubble[k].x, bubble[k].y, " ") == -1) {
          printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
        }

        bubble[k].x += bubble[k].state ? 1 : -1;
        bubble[k].state = 1 - bubble[k].state;
        bubble[k].y--;

        if (bubble[k].y <= 0) {
            bubble[k].y = LINES;
        }

        if (lo_send(t, "/print", "iis", bubble[k].x, bubble[k].y, glyph[i]) == -1) {
          printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
        }
    }

    runtime += WAIT * NBUBLE / 1000000.0;

    if (runtime > LENGTH) return 0;
  }
}
