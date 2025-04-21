#include <lo/lo.h>
#include <lo/lo_cpp.h>

#include <cassert>
#include <functional>
#include <iostream>

int message(const char* path, const char* types, lo_arg** argv, int argc,
            lo_message data, void* user_data) {
  int i;

  printf("generic handler; path: <%s>\n", path);
  for (i = 0; i < argc; i++) {
    printf("arg %d '%c' ", i, types[i]);
    lo_arg_pp((lo_type)types[i], argv[i]);
    printf("\n");
  }
  printf("\n");
  fflush(stdout);

  return 1;
}

int main() {
  auto server = lo_server_thread_new_multicast(
      "224.0.7.23", "7770", [](int n, const char* message, const char* where) {
        std::cout << "ERROR: " << message << "(" << where << ")" << std::endl;
        fflush(stdout);
      });
  lo_server_thread_add_method(server, nullptr, nullptr, message, nullptr);

  // XXX trying to pass a lambda... FAIL
  lo_server_thread_add_method(
      server, "/f", "s",
      std::function<lo_method_handler>([&](const char* path, const char* types, lo_arg** argv, int argc,
          lo_message data, void* user_data) -> int { return 1; })

      ,
      nullptr);

  lo_server_thread_start(server);
  getchar();
}
