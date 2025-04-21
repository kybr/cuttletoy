#include "Conf.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string &s) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string item;
  while (ss >> item) {
    result.push_back(item);
  }
  return result;
}

bool Conf::load() {
  std::string mac;
  {
    std::ifstream file("/sys/class/net/eth0/address");
    if (!file.is_open()) return false;
    if (!std::getline(file, mac)) return false;
  }
  printf("i am pi: %s\n", mac.c_str());

  {
    std::ifstream file("toy.conf");
    if (!file.is_open()) {
      printf("No 'toy.conf' file found\n");
      return false;
    }
    std::string line;
    while (std::getline(file, line)) {
      auto entry = split(line);
      if (entry.size() < 1) {
        continue;  // skip this line
      }
      if (mac == entry[0]) {
        // for (auto& e : entry) {
        //   printf("%s\n", e.c_str());
        // }
        if (entry.size() != 4) {
          printf("Wrong number of records in 'toy.conf'\n");
          return false;
        }
        try {
          x_screen = std::stoi(entry[1]);
          y_screen = std::stoi(entry[2]);
          id = std::stoi(entry[3]);
        } catch (...) {
          printf("Problem with 'toy.conf'\n");
          return false;
        }
        return true;
      }
    }
  }
  return false;
}

void Conf::show() {
  if (id < 0) {
    printf("Not running on any pi\n");
  }
  else {
    printf("Running on pi %d ... column%d row%d\n", id, x_screen, y_screen);
  }
}
