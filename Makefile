.RECIPEPREFIX = |
CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb
SRC=src/main.c src/recdir.c src/sha256.c

removedup: $(SRC)
| $(CC) $(CFLAGS) -o removedup $(SRC) $(LIBS) 
