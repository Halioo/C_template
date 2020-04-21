#
# Template de code C - Makefile des librairies du projet
#
# @author Matthias Brun, Clément Puybareau
# @copyright CCBY 4.0
#

# Lib packages
# TODO append your package name to the list
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