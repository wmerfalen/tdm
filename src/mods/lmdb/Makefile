GCC				= g++-7
NAME			= ghost
src       = .
builddir		= $(src)/build
OUTPUT			= ${builddir}/${NAME}.so
OBJECTS_DIR		= ${src}/objects
COMPILER_FLAGS	= -std=c++17 -I${src}/GSL/include -DGHOSTDNS_CONFIG_FILE=\"${src}/ghost.conf\" -Wall -O2 -fPIC
DAEMON_LINKER_FLAGS 	= -lrt

all: daemon viewer
	${GCC} ${COMPILER_FLAGS} gethostbyname.cpp -shared -o ${OUTPUT} -ldl 

daemon: daemon.cpp lmdb.so
	${GCC} ${COMPILER_FLAGS} ${src}/liblmdb/liblmdb.so daemon.cpp -o ${OBJECTS_DIR}/ghostdnsd ${DAEMON_LINKER_FLAGS}

lmdb.so:
	cd ${src}/liblmdb/ && make
viewer: shm-viewer.cpp
	${GCC} ${COMPILER_FLAGS} shm-viewer.cpp -o ${OBJECTS_DIR}/viewer ${DAEMON_LINKER_FLAGS}
clean:
	rm ${builddir}/*
	rm ${OBJECTS_DIR}/*
