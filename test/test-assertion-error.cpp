#include <cassert>

#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include <iostream>

int main(int argc, char* argv[]) {
  lo::ServerThread server(
      9000, [](int n, const char* message, const char* where) {
        std::cout << "ERROR: " << message << "(" << where << ")" << std::endl;
        fflush(stdout);
      });
  assert(server.is_valid());

  return 0;
}
