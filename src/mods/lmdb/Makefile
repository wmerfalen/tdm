GCC				= g++-7
NAME			= ghost
src       = $(CURDIR)/mods/lmdb
builddir		= $(src)/build
OUTPUT			= $(builddir)/$(NAME).so
OBJECTS_DIR		= $(src)/objects
COMPILER_FLAGS	= -std=c++17 -I$(CURDIR)/mods/include -DGHOSTDNS_CONFIG_FILE=\"$(src)/ghost.conf\" -Wall -O2 -fPIC
DAEMON_LINKER_FLAGS 	= -lrt

all: daemon.o viewer
	mkdir -p $(OBJECTS_DIR)/ghostdnsd && mkdir -p $(src)/ghostdnsd && $(GCC) $(COMPILER_FLAGS) $(src)/gethostbyname.cpp -shared -o $(OUTPUT) -ldl 

daemon.o: lmdb.so
	$(GCC) $(COMPILER_FLAGS) $(src)/liblmdb/liblmdb.so $(src)/daemon.cpp -o $(OBJECTS_DIR)/daemon.o $(DAEMON_LINKER_FLAGS)

lmdb.so:
	cd $(src)/liblmdb/ && make
viewer: shm-viewer.cpp
	$(GCC) $(COMPILER_FLAGS) $(src)/shm-viewer.cpp -o $(OBJECTS_DIR)/viewer $(DAEMON_LINKER_FLAGS)
clean:
	rm $(builddir)/*
	rm $(OBJECTS_DIR)/*
