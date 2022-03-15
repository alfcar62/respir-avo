CXX = gcc
LD  = ld

CFLAGS  = -O3 -Wall -Wextra -g
LDFLAGS = 

SRCDIR     = src
BINDIR     = bin
OUTFILE    = uniscimisure
OUTFILEEXT = 

ifeq ($(OS), Windows_NT)
	OUTFILEEXT = .exe
endif

all:
	$(CXX) $(CFLAGS) -o $(BINDIR)/$(OUTFILE)$(OUTFILEEXT) $(SRCDIR)/main.c

run:
	$(BINDIR)/$(OUTFILE)$(OUTFILEEXT)

setup:
	@ mkdir $(BINDIR)
