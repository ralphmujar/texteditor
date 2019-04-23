#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ncurses.h>
#include"b.h" //buffer

FILE *fp;
char **b;
int x, y, c, row, col;


char **editBuffer();

int right();
int left();
int up();
int down();
int save();

int insert(int c);

int main(int argc, char *argv[])
{
	int limit = 30;

	x = y = 0;
	row =0;
	col=0;

	fp = fopen(argv[1], "r");

	if (fp == NULL) {
		perror("no file\n");
	} else {
		char s[60];
		
		b = calloc(limit, sizeof(char *));

		while(fgets(s, 60, fp) != NULL)
		{
			b[row] = calloc(limit, sizeof(char));
			b[row++] = strdup(s);
		}

		editBuffer();
	}

}

char **editBuffer()
{

	initscr();
	noecho();

	for (int i=0; i < row; i++) {
		printw("%s", *(b+i));		
	}
	move(y, x);

	WINDOW *w = newwin(1, 10, 90, 0);
	refresh();

	keypad(stdscr, 1);
	while ((c=getch()) != 19)
	{
		if (c == KEY_RIGHT) {
			right();
		} else if (c == KEY_LEFT) {
			left();
		} else if (c == KEY_UP) {
			up();
		} else if (c == KEY_DOWN) {
			down();
		} else if (c == 27) {
			 save();
		} else {
			insert(c);
		}
	
		mvwprintw(w, 0, 0,  "(%d, %d), %c", y, x, b[y][x]);
		wrefresh(w);
	}
	endwin();	


}

int right()
{
	move(y, x++);
	if (b[y][x] == '\0') {
		x=0; down();
	}
}

int left()
{
	move(y, x--);
	if (x < 0) {
		x=0;
	}
}

int up()
{
	move(y--, x);
	if (y < 0) {
		y=0;
	}
}

int down()
{
	move(y++, x);
	if (*(b+y) == NULL) {
		up();
	}
}

int save()
{
	FILE *fp2 = fopen("output.txt", "w+");
	int i = 0;

	while (*(b+i))
	{

		fputs(*(b+i), fp2);
		i++;
	}
	fflush(fp2);

	WINDOW *w2 = newwin (1, 10, 90, 10);
	wprintw(w2, "save!");
	wrefresh(w2);
}

int insert(int c)
{
	b[y][x] = c;
	mvaddch(y, x, c);
	right();
}
