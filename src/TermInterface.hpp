#ifndef TERMINTERFACE_HPP
#define TERMINTERFACE_HPP

#include <ncurses.h>

#include "AIEngine.hpp"

class TermInterface
{
public:
  TermInterface();
  virtual ~TermInterface();

  void Show();
private:
  WINDOW* createWin(int height, int width, int y, int x);
  void destroyWin(WINDOW* win);
};

#endif