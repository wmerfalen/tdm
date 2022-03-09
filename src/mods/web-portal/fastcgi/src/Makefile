src = .
builddir = $(CURDIR)/build
BINDIR = $(CURDIR)/bin
CC = /usr/bin/g++-8
gdbflags = -ggdb -g
optimization = -O2
warnings_list = -Wall -Wunused-result \
	-Wno-format-zero-length -Wno-format-security \
	-Wuninitialized \
	-Wno-char-subscripts -Wcomment \
	-Wno-sign-compare 

FLAGS = $(gdbflags) $(optimization) \
				$(warnings_list) \
	-std=c++2a -fconcepts -fpermissive -MMD \
	-fPIC 

LIBS =  -lc -lm -lpqxx -lpq -L/usr/local/lib -lzmq -lpthread -lfcgi++ -lfcgi
buildobj = main

objects = $(patsubst %,$(builddir)/%.o,$(buildobj))
default: main
main: $(builddir) $(BINDIR) main
	$(CC) $(FLAGS) -o $(BINDIR)/fastcgi.bin main.cpp $(LIBS)

clean:
	make -f $(lmdbdir)/Makefile clean ; \
		rm -f $(addprefix $(builddir)/,*.o *.d)

$(builddir):
	mkdir -p $(builddir)

$(BINDIR):
	mkdir -p $(BINDIR)
