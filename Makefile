
LIBS = -lspdlog -lfmt
DEFINES = 


all:  genome.o GA.o test.o functions.o
	@g++ $(DEFINES) obj/test.o obj/GA.o obj/genome.o obj/functions.o $(LIBS) -o release/test
	@release/test

GA.o:
	@g++ -c GA.cpp -o obj/GA.o

genome.o:
	@g++ -c genome.cpp -o obj/genome.o

functions.o:
	@g++ -c functions.cpp -o obj/functions.o


test.o:
	@g++ -c test.cpp -o obj/test.o