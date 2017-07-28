CC = gcc

IDIR = include
SDIR = src
ODIR = $(SDIR)/obj

CFLAGS = -std=c99 -I $(IDIR) -Wall
LIBS = -lncurses

ENAME = tetris_clone

_DEPS = actions.h colors.h controller.h drawer.h game.h grid.h helpers.h tetramino.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = controller.o drawer.o game.o grid.o helpers.o main.o tetramino.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: $(OBJ)
	$(CC) -o $(ENAME) $^ $(CFLAGS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm $(ODIR)/*.o $(ENAME)
