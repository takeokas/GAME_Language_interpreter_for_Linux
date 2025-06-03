#

CFLAGS =  -DEMACS

all: gi

#
#	GAME interpriter
#
TOBJS =	tst.o lined.o linux.o

OBJS =	ggii.o lined.o linux.o
SRCS =	ggii.c lined.c linux.c
#OBJS =	ggii.o lined.o bsd.o
#SRCS =	ggii.c lined.c bsd.c
#OBJS =	ggii.o  bsd.o
#SRCS =	ggii.c  bsd.c

gi: ${OBJS}
	${CC} ${CFLAGS} -o gi ${OBJS}

ggii.o:	ggii.c
	${CC} ${CFLAGS} -c ggii.c

#
#	install
#
install:

#
#	make shar
#
shar:
	shar	readme.j Makefile $(SRCS) > gi.shar


#
#	clean
#
clean:
	rm -f *.o gi \#* *~

