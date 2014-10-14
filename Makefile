# usage: if this file is named "Makefile", then the commands are:
#	"make" will build the specified executable (PROG)
#	"make clean" will delete all of the .o and .exe files
#
# if this file is named something else, then use the -f option for make:
#	"make -f makefilename clean"

# set the variables below depending on compiler and options

CC = g++
LD = g++

# add -g to the CFLAGS for debugging (or -ggdb if you use gdb). 
# -pedantic-errors attempts to enforce Standard
# -Wall asks for certain warnings of possible errors
# -c is required to specify compile-only (no linking)

CFLAGS = -std=c++11 -pedantic-errors -Wall -c

#no load flags defined, but -l would be used to include a special library
LFLAGS = 

OBJS = p2_main.o p2_globals.o Room.o Person.o Meeting.o Utility.o
PROG = proj2exe

default: $(PROG)

debug: CFLAGS += -g -gdwarf-3 -fno-elide-constructors
debug: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

p2_main.o: p2_main.cpp p2_globals.h Room.h Meeting.h Person.h Utility.h
	$(CC) $(CFLAGS) p2_main.cpp

Room.o: Room.cpp Room.h Meeting.h Utility.h
	$(CC) $(CFLAGS) Room.cpp

Meeting.o: Meeting.cpp Meeting.h Person.h Utility.h
	$(CC) $(CFLAGS) Meeting.cpp

Person.o: Person.cpp Person.h Utility.h
	$(CC) $(CFLAGS) Person.cpp

p2_globals.o: p2_globals.cpp p2_globals.h 
	$(CC) $(CFLAGS) p2_globals.cpp

Utility.o: Utility.cpp Utility.h
	$(CC) $(CFLAGS) Utility.cpp

clean:
	rm -f *.o
real_clean:
	rm -rf *.o $(PROG)
