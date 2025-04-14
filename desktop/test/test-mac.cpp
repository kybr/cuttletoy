#include "Conf.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
  Conf conf;
  if (!conf.load()) {
    printf("No configuration loaded\n");
  }
  else {
    conf.show();
  }
}
