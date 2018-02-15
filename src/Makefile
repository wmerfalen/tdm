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

# Path to cxref utility
                                     CXREF = cxref

# Any special flags you want to pass to the compiler
                                             MYFLAGS = -Wall -Wno-char-subscripts -Wcomment

#flags for profiling (see hacker.doc for more information)
                                                       PROFILE =

                                                           BINDIR = ../bin

                                                                    CFLAGS = -g -O2 $(MYFLAGS) $(PROFILE)


                                                                             MODS_SRC :
                                                                             = $(wildcard mods/*.cpp)
MODS_OBJ := $(patsubst mods/%.cpp,mods/%.o,$(MODS_SRC))
MODS_CLASSES_SRC := $(wildcard mods/classes/*.cpp)
MODS_CLASSES_OBJ := $(patsubst mods/classes/%.cpp,mods/classes/%.o,$(MODS_CLASSES_SRC))
CPP_SRC := $(wildcard *.cpp)
CPP_OBJ := $(patsubst %.cpp,%.o,$(CPP_SRC))
C_SRC := $(wildcard *.c)
C_OBJ := $(patsubst %.c,%.o,$(C_SRC))

LIBS =  -lcrypt -lpqxx -lpq $(THIS_DIR)/mods/lib/liblmdb.so

#TODO add lmdb.so
OBJFILES = acl.o duktape.o $(MODS_OBJ) $(CPP_OBJ) $(C_OBJ) $(MODS_CLASSES_OBJ)

CXREF_FILES = act.comm.c act.informative.c act.item.c act.movement.c \
	act.other.c act.social.c act.wizard.cpp alias.c ban.c \
	boards.c castle.c class.c comm.c config.c constants.c fight.c \
	graph.c handler.c house.c interpreter.c limits.c magic.c mail.c \
	mobact.c modify.c objsave.c olc.c random.c shop.c spec_assign.c\
	spec_procs.c spell_parser.c spells.c utils.c weather.c \
	bsd-snprintf.c globals.cpp mods/acl/config-parser.cpp \
	mods/utils.hpp mods/scan.hpp


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
ref:
#
# Create the cross reference files
# Note, this is not meant to be used unless you've installed cxref...
#
	@for file in $(CXREF_FILES) ; do \
	  echo Cross referencing $$file ; \
	  $(CXREF) -D__CXREF__ -xref -Odoc -Ncircle $$file ; \
	done
#
# Create the source files using cxref
#
	@for file in $(CXREF_FILES) ; do \
	   echo Documenting $$file ; \
	   ( cd . ; $(CXREF) -D__CXREF__ -warn-xref -xref -Odoc -Ncircle -html $$file ) ; \
	   rm -f $(DOCS) ; \
	done
#
# Create the index using cxref
#
	@echo Indexing
	@( cd . ; $(CXREF) -D__CXREF__ -index-all -Odoc -Ncircle -html )
	@rm -f $(DOCS)
#
# Make html files for the .h files
#
	@echo Creating .h.html files...
	@for file in *.h ; do \
	  echo $$file ; \
	  cat htmlh-head $$file htmlh-tail > doc/$$file.html ; \
	done
# Copy over to the html directory
	#cp doc/*.html $(HOME)/www/cxref
	#chmod 644 $(HOME)/www/cxref/*.html

# Dependencies for the object files (automagically generated with
# gcc -MM)

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
