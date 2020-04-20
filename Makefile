#
# Robot v1 C - Makefile principal.
#
# @author Matthias Brun, adapté par Jérôme Delatour
#

# Organisation des sources.
#

export SRCDIR = src
export LIBDIR = $(realpath lib)
export BINDIR = bin

SUBDIRS = $(LIBDIR)
SUBDIRS += $(SRCDIR)

#
# Définitions des outils.
#

# Compilateur.

export CC = gcc

# options de compilation
export CCFLAGS += -O0
# avec debuggage : -g -DDEBUG
#export CCFLAGS += -g -DDEBUG
# sans debuggage : -DNDEBUG
export CCFLAGS += -DNDEBUG
 # gestion automatique des dépendances
export CCFLAGS += -MMD -MP
export CCFLAGS += -D_BSD_SOURCE -D_XOPEN_SOURCE_EXTENDED -D_XOPEN_SOURCE -D_DEFAULT_SOURCE -D_GNU_SOURCE
# On compile en utilisant les options de gcc
export CCFLAGS += -std=c99 -Wall
# -pedantic retiré car génère des warnings pour mes TRACE

#options de compilation pour l'utilisation des libs personnalisées
export CCFLAGS += -I$(LIBDIR)/include/
export LDFLAGS += -L$(LIBDIR)/watchdog/
export LDFLAGS += -L$(LIBDIR)/mailbox/
export LDFLAGS += -lwatchdog -lmailbox
export LDFLAGS += -lrt -pthread

#
# Définitions du binaire à générer.
#
export PROG = $(BINDIR)/prog

#
# Règles du Makefile.
#

# Compilation récursive.
all:

	@for i in $(SUBDIRS); do (cd $$i; make $@); done

# Nettoyage.
.PHONY: clean

clean:
	@for i in $(SUBDIRS); do (cd $$i; make $@); done
	@rm -f $(PROG) core* $(BINDIR)/core*

