#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ncurses.h>
#include"b.h" //buffer

FILE *fp;
char **b;
gapBuf *gb, gbuff;
int x, y, c, row, col, slen, limit;


char **editBuffer();
char **emptyBuffer();
int keyPress();

int right();
int left();
int up();
int down();
int save();

int moveGap();
int insert(int c);
int shiftRight();
int shiftLeft();
int enter();
int tab();
void println();

int main(int argc, char *argv[])
{
	
	limit = 30;

	gb = &gbuff;
	
	x = y = 0;
	row =0;
	col=0;

	fp = fopen(argv[1], "r");
	b = calloc(limit, sizeof(char *));

//	if (fp == NULL) {
		//perror("no file\n");
	if (fp == NULL) {

		b[row] = malloc(limit * sizeof(char));
		emptyBuffer();

	} else {
		char s[60];
		
		//b = calloc(limit, sizeof(char *));

		while(fgets(s, 60, fp) != NULL)
		{
			b[row] = calloc(limit, sizeof(char));
			b[row++] = strdup(s);
		}

		editBuffer();
	}

	free(b);
	return 0;

}

char **emptyBuffer()
{

	initscr();
	move(y, x);
	keypad(stdscr, 1);
	keyPress();
	endwin();
}



char **editBuffer()
{

	initscr();

	for (int i=0; i < row; i++) {
		printw("%s", *(b+i));		
	}
	move(y, x);

	keypad(stdscr, 1);
	keyPress();
	endwin();	


}

int keyPress() {

	WINDOW *w = newwin(1, 10, 90, 0);
	WINDOW *w_gap = newwin(1, 20, 90, 40);
	refresh();

	while ((c=getch()) != 19)
	{
		if (c == KEY_RIGHT) {
			right();
		} else if (c == KEY_LEFT) {
			left();
		} else if (c == KEY_UP) {
			up();
		} else if (c == KEY_DOWN) {
			x=0;
			down();
		} else if (c == KEY_BACKSPACE) {

			left();
			mvdelch(y, x);
			shiftLeft();
	
		} else if (c == 9) {
			tab(c);
		} else if (c == 10) {

			x=0; down();
			enter(c);
		} else if (c == 27) {
			 save();


		} else {
			insert(c);
		}
	
		mvwprintw(w, 0, 0,  "(%d, %d), %c", y, x, b[y][x]);
		mvwprintw(w_gap, 0, 0,  "tr=%d", row);
		wrefresh(w);
		wrefresh(w_gap);
	}


}

int right()
{
	//buffer+1 screen+2
	move(y, (x++)+1);
	//if (x > (strlen(b[y]) + 1)) {
	if (b[y][x] == 0) {
		x=0; down();
	}
}

int left()
{
	move(y, (x--)-1);
	//moveGap();
	if (x < 0) {
		if (y <= 0) {
			down();
		} else {
			x = (strlen(b[y-1]) - 1);
			up();
		}
	}
}

int up()
{
	move((y--)-1, x);
	//moveGap();
	if (y < 0) {
		y=0;
	}
}

int down()
{
	move((y++)+1, x);
	//moveGap();
	//if (*(b+y) == 0) {
	if (*(b+y) == 0) {
		up();
	}
}

int insert(int c)
{
	if (b[y][x+1] != 0) {

		shiftRight();
		b[y][x] = c;
		mvaddch(y, x, c);
		right();

	} else {

		b[y][x] = c;
		println();
	}
}

int moveGap()
{
	gb->start = x;
	gb->end = (x+3);
	gb->size = (gb->end - gb->start);
}



int shiftRight()
{
	int n, temp, s, i;

	n = strlen(b[y]) + 1;
	temp = b[y][n-1];
	s = b[y][x];

	for (i=n-2; i > x; i--){
		b[y][i+1] = b[y][i];
	}

	b[y][n+1] = temp;
	b[y][x+1] = s;

	for (i=x; i < n; i++) {
		mvprintw(y, i, "%c", b[y][i]);
	}

}

int tab(int c)
{
	int n, i;
	n = 8;
	shiftRight();
	b[y][x] = c;
	right();

	for (i=0; i < n; i++) {
		shiftRight();
		b[y][x] = 32;
		right();
	}

	for (i=0; i < (strlen(b[y])+1); i++) {
		mvprintw(y, i, "%c", b[y][i]);
	}
	move(y, x);
}


int shiftLeft()
{
	int i, n;
	n = (strlen(b[y]) + 1);
	for (i=x; i <= n; i++){
		b[y][i] = b[y][i+1];
	}
}


int enter()
{
	int i;
	/*char temp[30];
	strcpy(temp, b[row-1]);*/

	if (b[y] != NULL) {

		if (b[row] == NULL) {
			
			b[row] = calloc(limit, sizeof(char));
			row++;
			
			for(i = row-1; i >=  y; i--) {

				strcpy(b[i], b[i-1]);
			}		
			memset(*(b+y), 0, sizeof(b[y]));
			b[y][0] = c;	
		}

	} else {

		b[y] = calloc(limit, sizeof(char));
		row++;

		memset(*(b+y), 0, sizeof(b[y]));
		b[y][0] = c;	
	}

	for (i=y; i < row; i++) {
		printw("%s", *(b+i));
	}
	move(y, 0);
}

void println()
{
	for (int i=0; i < (strlen(b[y])+1); i++) {
		mvprintw(y, i, "%c", b[y][i]);
	}
	move(y, (x++)+1);
}

int save()
{
	FILE *fp2 = fopen("o.txt", "w+");
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
