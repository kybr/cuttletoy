#include "Conf.hpp"

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

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
	 continue; // skip this line
      }
      if (mac == entry[0]) {
        // for (auto& e : entry) {
        //   printf("%s\n", e.c_str());
	// }
        if (entry.size() != 9) {
          printf("Wrong number of records in 'toy.conf'\n");
          return false;
        } 
        try {
          x_display = std::stoi(entry[1]);
          y_display = std::stoi(entry[2]);
          x_display_maximum = std::stoi(entry[3]);
          y_display_maximum = std::stoi(entry[4]);
          x_pixel_offset = std::stoi(entry[5]);
          y_pixel_offset = std::stoi(entry[6]);
          x_pixel_offset_maximum = std::stoi(entry[7]);
          y_pixel_offset_maximum = std::stoi(entry[8]);
	}
	catch (...) {
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
  printf("display: (%d,%d) of (%d,%d)\n", x_display, y_display, x_display_maximum, y_display_maximum);
  printf("pixel offset: (%d,%d) of (%d,%d)\n", x_pixel_offset, y_pixel_offset, x_pixel_offset_maximum, y_pixel_offset_maximum);
}
