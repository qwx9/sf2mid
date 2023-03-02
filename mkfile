</$objtype/mkfile

BIN=/$objtype/bin/games
TARG=sf2mid
OFILES=\
	sf2mid.$O\

HFILES=\
	tml.h\
	tsf.h\

CC=pcc
#CFLAGS=-vwc
CFLAGS=$CFLAGS -c -p -D_POSIX_SOURCE

</sys/src/cmd/mkone
