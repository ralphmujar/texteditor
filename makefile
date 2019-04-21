c=gcc
a: a.c
	$(c) -g -o a a.c buffer.h -lncurses
