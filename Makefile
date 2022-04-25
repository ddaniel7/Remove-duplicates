.RECIPEPREFIX = |
CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb


removedup: src/main.c
| $(CC) $(CFLAGS) -o removedup src/main.c $(LIBS) 
