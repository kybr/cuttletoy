#pragma once

struct Conf {
  int x_screen;
  int y_screen;
  int x_screen_maximum;
  int y_screen_maximum;
  int x_pixel_offset;
  int y_pixel_offset;
  int x_pixel_offset_maximum;
  int y_pixel_offset_maximum;
  bool load();
  void show();
};