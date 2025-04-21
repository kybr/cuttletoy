#pragma once

struct Conf {
  int x_screen = 0;
  int y_screen = 0;
  int id = 0;
  bool load();
  void show();
};