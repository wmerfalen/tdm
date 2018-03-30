# Generated automatically from Makefile.in by configure.
# CircleMUD Makefile.in - Makefile template used by 'configure'
#
THIS_DIR = $(shell pwd)
# C compiler to use
CC = g++-7 -g -A -da -std=c++17 -fpermissive -MMD \
-I$(THIS_DIR)/mods/include \
-L$(THIS_DIR)/mods/lib \
-I$(THIS_DIR)/boost-cpp \
-L$(THIS_DIR)/boost-cpp/build/lib

# Any special flags you want to pass to the compiler
MYFLAGS = -Wall -Wno-char-subscripts -Wcomment

#flags for profiling (see hacker.doc for more information)
PROFILE =

BINDIR = ../bin

CFLAGS = -g -O2 $(MYFLAGS) $(PROFILE)


MODS_SRC := $(wildcard mods/*.cpp)
MODS_OBJ := $(patsubst mods/%.cpp,mods/%.o,$(MODS_SRC))
MODS_CLASSES_SRC := $(wildcard mods/classes/*.cpp)
MODS_CLASSES_OBJ := $(patsubst mods/classes/%.cpp,mods/classes/%.o,$(MODS_CLASSES_SRC))
CPP_SRC := $(wildcard *.cpp)
CPP_OBJ := $(patsubst %.cpp,%.o,$(CPP_SRC))

LIBS =  -lcrypt -lpqxx -lpq $(THIS_DIR)/mods/lib/liblmdb.so

#TODO add lmdb.so
OBJFILES = acl.o duktape.o $(MODS_OBJ) $(CPP_OBJ) $(MODS_CLASSES_OBJ)

default: all

all: .accepted
	$(MAKE) $(BINDIR)/circle
	#$(MAKE) utils

.accepted:
	@./licheck less

utils: .accepted
	(cd util; $(MAKE) all)
circle:
	$(MAKE) $(BINDIR)/circle

$(BINDIR)/circle : $(OBJFILES)
	$(CC) -o $(BINDIR)/circle $(PROFILE) $(OBJFILES) $(LIBS)

clean:
	rm -f *.o
	rm -f mods/*.o
	rm -f mds/classes/*.o

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $<
mods_%.o: mods/%.cpp
	$(CC) -c $(CFLAGS) -o $@ $<
mods_classes_%.o: mods/classes/%.cpp
	$(CC) -c $(CFLAGS) -o $@ $<
duktape.o: duktape-2.2.0/src/duktape.h
	$(CC) -c $(CFLAGS) duktape-2.2.0/src/duktape.c -o duktape.o
acl.o: mods/acl/config-parser.cpp
	$(CC) -c $(CFLAGS) mods/acl/config-parser.cpp -o acl.o
liblmdb.so:
	#todo
