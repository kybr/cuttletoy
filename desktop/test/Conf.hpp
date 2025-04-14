#pragma once

struct Conf {
  int x_display;
  int y_display;
  int x_display_maximum;
  int y_display_maximum;
  int x_pixel_offset;
  int y_pixel_offset;
  int x_pixel_offset_maximum;
  int y_pixel_offset_maximum;
  bool load();
  void show();
};
