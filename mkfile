PREFIX=/usr/local

CC=cc
LD=$CC

CPPFLAGS=-DNDEBUG -D_POSIX_SOURCE=200809L

lb: lb.o util.o
	$LD -o$target $LDFLAGS $prereq $LIBS

%.o: %.c
	$CC -c $CFLAGS $CPPFLAGS $stem.c

clean:V:
	rm -f *.o lb
nuke:V: clean

install:V:
	cp lb $DESTDIR$PREFIX/bin/lb
	chmod 555 $DESTDIR$PREFIX/bin/lb
