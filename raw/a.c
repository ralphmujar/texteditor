#include<ncurses.h>
#include<stdlib.h>
#include<stdio.h>

int save();
int edit();


int c, x, y, i, e;
char buffer[100];
FILE *fp;


int main(int argc, char *argv[]) {

  x = y = 0;
  e = 0;
 
  if (argc > 1) {
    fp = fopen(argv[1], "r");
    fgets(buffer, 100, fp);

    initscr();
    noecho();
    mvprintw(y, x, "%s", buffer);
    edit();

       
  } else {
    initscr();
    noecho();
    edit();
  }
  
  return 0;
}


int edit()
{


  keypad(stdscr, true);

    
  while ((c=getch()) != '$') {

    if (c == KEY_BACKSPACE) {
      x--;
      mvdelch(y, x);
      e--;
      buffer[e] = c;
    } else if (c == KEY_UP) {
      y--;
      move(y, x);
    } else if (c == KEY_DOWN) {
      y++;
      move(y, x);
    } else if (c == KEY_RIGHT) {
      x++;
      e++;
      move(y, x);
    } else if (c == KEY_LEFT) {
      x--;
      e--;
      move(y, x);
    } else if (c == 10) { //enter
      y++;
      x = 0;    
      move(y, x);
      buffer[e] = c;
      e++;
    } else if (c == 27) { // s
      save();
    } else {
      addch(c);
      x++;
      buffer[e] = c;
      e++;
    }
  };

   endwin();
}


int save()
{
  fp = fopen("x.c", "w");
  for (i = 0; i < e; ++i) {
     fprintf(fp, "%c", buffer[i]);
  }
  fclose(fp);
  /*for (i = 0; i <= e; ++i) {
     printw("%c-", buffer[i]);
     }*/
  
}


