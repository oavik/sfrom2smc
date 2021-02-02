.POSIX:

CC=c99
LD=$(CC)
CFLAGS=-Wall -Wextra -Wpedantic -Werror -g
LDFLAGS=
LDLIBS=
SRCDIR=.
OBJDIR=.
BINDIR=$(OBJDIR)
DESTDIR=/usr/local

all: $(BINDIR)/sfrom2smc

clean:
	rm -f $(BINDIR)/sfrom2smc $(OBJDIR)/*.o

install: $(BINDIR)/sfrom2smc
	cp $(BINDIR)/sfrom2smc $(DESTDIR)/bin

$(BINDIR)/sfrom2smc: $(OBJDIR)/sfrom2smc.o
$(OBJDIR)/sfrom2smc.o: $(SRCDIR)/sfrom2smc.c
	$(CC) $(CFLAGS) -o $@ -c $(SRCDIR)/sfrom2smc.c

$(BINDIR)/sfrom2smc:
	$(LD) $(LDFLAGS) -o $@ $(OBJDIR)/*.o $(LDLIBS)
