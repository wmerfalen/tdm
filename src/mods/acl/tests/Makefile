GCC = g++
FLAGS = -std=c++14 -Wall

#test-command: command.o
#	$(GCC) $(FLAGS) command.o testcommand.cpp -o tcommand-main

#command.o:
#	$(GCC) $(FLAGS) -c ../command.cpp -o command.o
#
fileparser: fileparser.o
	$(GCC) $(FLAGS) fileparser.o testfileparser.cpp -o fileparser

fileparser.o:
	$(GCC) $(FLAGS) -c ../config-parser.cpp -o fileparser.o
