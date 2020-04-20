#
# Hello Robot C - Makefile du package robot des sources.
#
# @author Matthias Brun
#

#
# Organisation des sources.
#

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

# Inclusion depuis le niveau du package.
CCFLAGS += -I..

#
# Règles du Makefile.
#

# Compilation.
all: $(OBJ)

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@

# Nettoyage.
.PHONY: clean

clean:
	@rm -f $(OBJ) $(DEP)

-include $(DEP)

