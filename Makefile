MAKEFLAGS += -j4


LIBS =  -lboost_log -lboost_log_setup  -lboost_system -lboost_thread -lpthread 
DEFINES = 
OPT = -static 

CC = g++

LIBML = -Linclude/libml/lib/ -lml

CFlags = -g -Wall


all: genome.o GA.o test.o GA_functions.o gen_functions.o GA_helpers.o
	$(CC) $(CFlags) $(OPT) $(DEFINES) obj/test.o obj/GA.o obj/genome.o obj/GA_functions.o obj/gen_functions.o obj/GA_helpers.o $(LIBS) -o release/test
	#@$(CC) $(OPT) $(DEFINES) obj/test.o obj/GA.o obj/genome.o obj/GA_functions.o obj/gen_functions.o $(LIBS) -o debug/test
	#@release/test

GA.o:
	$(CC) $(CFlags) -c GA.cpp -o obj/GA.o

genome.o:
	$(CC) $(CFlags) -c genome.cpp -o obj/genome.o

GA_functions.o:
	$(CC) $(CFlags) -c GA_functions.cpp -o obj/GA_functions.o

gen_functions.o:
	$(CC) $(CFlags) -c gen_functions.cpp -o obj/gen_functions.o

GA_helpers.o:
	$(CC) $(CFlags) -c GA_helpers.cpp -o obj/GA_helpers.o



test.o:
	$(CC) $(CFlags) -c test.cpp -o obj/test.o


test2: test2.o
	$(CC) $(CFlags) $(OPT) $(DEFINES) obj/test2.o -o release/test2
test2.o:
	$(CC) $(CFlags) -c test2.cpp -o obj/test2.o


libml:
	$(MAKE) -Cinclude/libml/src/