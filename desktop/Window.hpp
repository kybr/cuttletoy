#pragma once

class WindowImp;
class Window {
  WindowImp* i;
 public:
  Window();
  ~Window();
  void swap();
  void size(int& width, int& height);
  bool done();
};
