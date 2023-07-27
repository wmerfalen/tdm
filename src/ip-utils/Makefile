all: client server
client: build
	g++-10 -std=c++2a client.cpp -o ./build/client
server: build
	g++-10 -std=c++2a server.cpp -o ./build/server

build:
	mkdir ./build
