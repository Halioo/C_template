#
# Hello Robot C - Makefile des sources du projet.
#
# @author Matthias Brun
#

#
# Organisation des sources.
#

# Packages du projet (à compléter si besoin est).
PACKAGES = example

# Un niveau de package est accessible.
SRC = $(wildcard *.c)
SRC += $(wildcard */*.c)
# Pour ajouter un second niveau :
# SRC += $(wildcard */*/*.c)

OBJ = $(SRC:.c=.o)

# Point d'entrée du programme.
MAIN = main.c

# Gestion automatique des dépendances.
DEP = $(MAIN:.c=.d)

# Exécutable à générer.
EXEC = ../$(PROG)


# Inclusion depuis le niveau du package.
CCFLAGS += -I.

#
# Règles du Makefile.
#

# Compilation.
all: main.o
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	$(CC) $(CCFLAGS) $(OBJ) -MF $(DEP) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CCFLAGS) $< -o $@

# Nettoyage.
.PHONY: clean

clean:
	@for p in $(PACKAGES); do (cd $$p; $(MAKE) $@); done
	@rm -f $(DEP)

-include $(DEP)
