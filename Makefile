.RECIPEPREFIX = |
CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb


removedup: main.c
| $(CC) $(CFLAGS) -o removedup main.c $(LIBS) 
