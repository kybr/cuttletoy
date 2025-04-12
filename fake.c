#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <lo/lo.h>
int main(int argc, char *argv[]) {
  lo_address t = lo_address_new(NULL, "7770");

  float angle = 0;
  int n = 0;
  while (1) {
    n++;
    if (n % 50 == 0) {
      if (lo_send(t, "/frag", "s", "this is a fragment shader") == -1) {
        printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
      }
    }

    angle += 0.033;
    int x = 500 * cos(angle) + 500;
    int y = 500 * sin(angle) + 500;

    if (lo_send(t, "/xy", "ii", x, y) == -1) {
      printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
    }

    int fps = 20;
    usleep(1000000.0/fps);
    printf("\33[2K\r");
    printf("%d", n);
    fflush(stdout);
  }
}
