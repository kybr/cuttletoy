#include <lo/lo.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("No file named\n");
    exit(1);
  }

  char* buffer = 0;
  long length;
  FILE* f = fopen(argv[1], "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc(length);
    if (buffer) {
      fread(buffer, 1, length, f);
    }
    fclose(f);
  }

  lo_address t = lo_address_new("224.0.7.23", "7770");
  if (lo_send(t, "/f", "s", buffer) == -1) {
    printf("send failed\n");
  }
}

