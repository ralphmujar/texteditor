#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ncurses.h>
#include"buffer.h"
//#include"key.c"



bufferSize *bs, cb;
gap *gapBuffer, gb;


FILE *fp;
char **b;


int x, y;
int c;


char **createBuffer();
void printBuffer(char **b);

int down();
int up();
int left();
int right();
int save();


int main(int argc, char *argv[])
{

	gapBuffer = &gb;

	bs = &cb;
	x=0;
	y=0;


	bs->r = 0;
	bs->c = 0;
	bs->rowLimit = 2;
	bs->colLimit = 4;
	
	fp = fopen(argv[1], "r");

	if (fp == NULL)
	{
		printf("no file\n");

	} else {

		b = createBuffer(bs);
		printBuffer(b);
		
	}
}

char **createBuffer(bufferSize *bs)
{
	//gapBuffer->start = 0;
	//gapBuffer->size = 3;

	b = calloc(bs->rowLimit, sizeof(int *));
	b[0] = calloc(bs->colLimit, sizeof(char));		
	int ch;


	while((ch=fgetc(fp)) != EOF)
	{
		b[bs->r][bs->c] = ch;
		bs->c = bs->c + 1;

		if (b[bs->r][bs->c] == 10)
		{
			bs->r = bs->r + 1;
			b[bs->r] = calloc(bs->colLimit, sizeof(char));
			bs->c = 0;
		}

		if (bs->c == bs->colLimit)
		{
			bs->colLimit = bs->colLimit * 2;
			b[bs->r] = realloc(b[bs->r], bs->colLimit);
		}
	}

	return b;
}

void printBuffer(char **b)
{
	int i;
	initscr();
	noecho();

	for (i = 0; i <= bs->r; i++)
	{	
		addstr(*(b+i));
	}

	printw("%d", bs->r);
	move(y, x);
	
	keypad(stdscr, true);
	while((c =getch()) != 19)
	{
		if (c == KEY_DOWN) {
			down();
		} else if (c == KEY_UP) {
			up();
		} else if (c == KEY_LEFT) {
			left();
		} else if(c == KEY_RIGHT) {
			right();
		} else if(c == 27) {
			save();
		} else {
				
		}
		

	}
	//refresh();
	endwin();
}

int down()
{	
	move(y++, x);
	//if(*(b+y) == NULL) {move(y--, x);}
}

int up()
{
	move(y--, x);
}

int left()
{
	move(y, x--);
	addch(b[y][x]);
}


int right()
{	
	addch(b[y][x]);
	move(y, x++);
}

int save()
{			
	FILE* fp2 = fopen("c.txt", "w");
		for(int i=0; i<=bs->r; i++)
			fputs(*(b+i), fp2);
}
