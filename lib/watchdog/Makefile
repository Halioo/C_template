#
# Hello Robot C - Makefile des sources du projet.
#
# @author Matthias Brun, Clément Puybareau
#

LIBNAME = watchdog

ARCHIVE = lib$(LIBNAME).a
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

# Inclusion depuis le niveau du package.


# Compilation.
all: $(OBJ)
	ar -rv $(ARCHIVE) $(OBJ)

%.o: %.c
	$(CC) -I../include/ -c $< -o $@