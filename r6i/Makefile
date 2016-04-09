libs = ncurses
target = r6i
#sources = arithm.c channel.c class.c concha.c conmask.c constor.c constr.c contain.c convect.c fileio.c nomouse.c novideo.c rbarm1.c rbarm.c rbclass.c rbcont.c rbeval.c rbio1.c rbio.c rbsys.c rbtable.c refgen.c refini.c rfalloc.c rfarm1.c rfarm.c rfinter.c rfserv.crfstor.c rfelst.c rfgo.c rfinit.c rfinp.c rfmain.c rfoutp.c rftable.c
sources = arithm.c channel.c class.c concha.c conmask.c constor.c constr.c contain.c convect.c fileio.c nomouse.c novideo.c rbarm.c rbclass.c rbcont.c rbeval.c rbio.c rbsys.c rbtable.c refgen.c refini.c rfalloc.c rfarm.c rfinter.c rfserv.c rfstor.c rfelst.c rfgo.c rfinit.c rfinp.c rfmain.c rfoutp.c rftable.c
#sources = $(wildcard *.c)
objects = $(patsubst %.c,%.o,$(sources))

CFLAGS = -c -std=c11 -Werror -gdwarf
LDFLAGS = -lncurses -lm -gdwarf

$(target): $(objects) ;	gcc $^ $(LDFLAGS) -o $@

$(objects): config.h

%.o: %.c ; gcc $(CFLAGS) $< -o $@

all: $(target)

.PHONY: clean

clean:
	-rm -f $(target) $(objects)
	-rm -f *.E
