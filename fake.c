#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <lo/lo.h>
int main(int argc, char *argv[]) {
  lo_address t = lo_address_new(NULL, "7770");

  float angle = 0;
  while (1) {
    angle += 0.033;
    int x = 500 * cos(angle) + 500;
    int y = 500 * sin(angle) + 500;

    if (lo_send(t, "/xy", "ii", x, y) == -1) {
      printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
    }

    usleep(16670);
  }
}
