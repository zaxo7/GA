MAKEFLAGS += -j1


LIBS =  -lboost_log -lboost_log_setup  -lboost_system -lboost_thread -lpthread 
#MUTATION_1 is the default mutation using a dampening_factor
#USE_SWAP_MUTATION is used to remove duplicates
DEFINES = -DUSE_SWAP_MUTATION #-DMUTATION_1
OPT = -static 

CC = g++

LIBML = -Linclude/libml/lib/ -lml

CFlags = -g -Wall

OBJDIR := ./obj/
GA_lib_obj := $(addprefix $(OBJDIR)/, GA.o GA_functions.o GA_helpers.o genome.o gen_functions.o)

all: | clear GA_lib  TSP.o test test2

GA_lib: GA.o GA_functions.o GA_helpers.o genome.o gen_functions.o

GA.o:
	$(CC) $(CFlags) $(DEFINES) -c GA.cpp -o obj/GA.o

genome.o:
	$(CC) $(CFlags) $(DEFINES) -c genome.cpp -o obj/genome.o

GA_functions.o:
	$(CC) $(CFlags) $(DEFINES) -c GA_functions.cpp -o obj/GA_functions.o

gen_functions.o:
	$(CC) $(CFlags) $(DEFINES) -c gen_functions.cpp -o obj/gen_functions.o

GA_helpers.o:
	$(CC) $(CFlags) $(DEFINES) -c GA_helpers.cpp -o obj/GA_helpers.o





TSP.o:
	$(CC) $(CFlags) $(DEFINES) -c TSP.cpp -o obj/TSP.o




test: test.o
	$(CC) $(CFlags) $(OPT) obj/test.o obj/TSP.o obj/GA.o obj/genome.o obj/GA_functions.o obj/gen_functions.o obj/GA_helpers.o $(LIBS) -o release/test
	@#@$(CC) $(OPT) $(DEFINES) obj/test.o obj/GA.o obj/genome.o obj/GA_functions.o obj/gen_functions.o $(LIBS) -o debug/test
	@#@release/test

test.o:
	$(CC) $(CFlags) $(DEFINES) -c test.cpp -o obj/test.o


test2: test2.o
	@#$(CC) $(CFlags) $(OPT) $(DEFINES) obj/test2.o -o release/test2
	$(CC) $(CFlags) $(OPT) obj/test2.o obj/TSP.o obj/GA.o obj/genome.o obj/GA_functions.o obj/gen_functions.o obj/GA_helpers.o $(LIBS) -o release/test2

test2.o:
	$(CC) $(CFlags) $(DEFINES) -c test2.cpp -o obj/test2.o


libml:
	$(MAKE) -Cinclude/libml/src/


clear:
	rm -f obj/*