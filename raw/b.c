#include<curses.h>

int main(int argc, char *argv[])
{
  initscr();
  int h, w, y, x;
  x = y = 2;
  h = 5;
  w = 10;
  WINDOW * win;

  win = newwin(h, w, y, x);
  refresh();
  
  box(win, 0, 0);
  wrefresh(win);
  keypad(win, true);
  int c = wgetch(win);
  if (c == KEY_UP) {
    mvwprintw(win, 1, 1, "up");
    wrefresh(win);
  }

  getch();
  endwin();
  return 0;
}
