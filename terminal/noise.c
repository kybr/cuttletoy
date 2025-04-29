#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <lo/lo.h>

#include "data/h/Unifont-APL8x16-16.0.03.psf.gz.txt.h"
//#include "data/h/Uni3-Terminus24x12.psf.gz.txt.h"

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

const int length = sizeof(glyph) / sizeof(glyph[0]);

int main() {
  //for (int k = 0; k < length; ++k) {
  //  printf("%d: %zu\n", k, strnlen(glyph[k], 10));
  //}

  srand(0);

  lo_address t = lo_address_new("224.0.7.23", "7770");
  while (1) {
    usleep(25000);
    // int x = 25 + 25 * rndf();
    // int y = 16 * 15 * rndf();
    // int i = rndu() % length;
    //int x = rndu() % 53; // 25 + 25 * rndf();
    //int y = rndu() % 32; // 16 * 15 * rndf();
    //int i = rndu() % length;
    int x = rand() % 240;
    int y = rand() % 67;
    int i = rand() % length;
    printf("%d: %s\n", i, glyph[i]);
    if (lo_send(t, "/print", "iis", x, y, glyph[i]) == -1) {
      printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
    }
  }
}
