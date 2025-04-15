#include <lo/lo.h>
#include <lo/lo_cpp.h>
#include <unistd.h>  // usleep

int main(int argc, char *argv[]) {
  lo::Address screen("localhost", 9000);
  int n = 0;
  while (true) {
    screen.send("/frag", "s", "this is a fragment shader");

    int fps = 5;
    usleep(1000000.0 / fps);
    printf("%d\n", n++);
    fflush(stdout);
  }
}
