#
# Hello Robot C - Makefile des sources du projet.
#
# @author Matthias Brun, Clément Puybareau
#

LIBRARIES = watchdog mailbox

# Inclusion depuis le niveau du package.
CCFLAGS += -I.

# Compilation.
all:
	@for p in $(LIBRARIES); do (cd $$p; $(MAKE) $@); done

# Nettoyage.
.PHONY: clean

clean:
	@for i in $(LIBRAIRIES); do (cd $$i; make $@); done
	@rm -f $(PROG) core* $(BINDIR)/core*