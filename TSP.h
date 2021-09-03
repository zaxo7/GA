#ifndef DEF_TSP
#define DEF_TSP

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <functional>

#include "GA.h"

#include "TSP_func.h"

enum GA_type
{
	SIMPLE_NN,SIMPLE_RND,DOUBLE_CHROMOSOME_NN,DOUBLE_CHROMOSOME_RND,NSE_NN,NSE_RND,NONE
};

using namespace std;

class TSP
{
private:

	string *benchName;
	string *benchPath;


	int benchSize;

	int popSize;
	int iterations;
	float mutationChance;

	GA_type solve_type;

	GA *GA_engine;

	ga_result *result; // will be returned

	int genomeLen;
	int codonMin;
	int codonMax;

	
	float** weightsMatrix;
	float** coordsMatrix;

	//initial tour matrix
	int **initial;
	int initial_count;

	//stops when it reachs this cost
	float *termination_cost;

	int elitism;

	monitor monitorFunc;
	eval evalFunc;
	selection selectionFunc;
	crossover crossoverFunc;
	mutation mutationFunc;

	bool allowrepeat;
	bool verbose;

	bool log;

	string logFile;

public:
	
	static string default_root;



	//constructors

	TSP();

	TSP(string benchmark, int popSize, int iterations, float mutationChance, bool fullPath = false);
	
	TSP(string benchmark, int popSize, int iterations, float mutationChance, GA_type solve_type, bool fullPath);

	TSP(string benchmark, int popSize, int iterations, float mutationChance, GA_type solve_type, monitor monitorFunc, eval evalFunc, selection selectionFunc, crossover crossoverFunc, mutation mutationFunc, bool fullPath);

	//this method start solving
	ga_result* solve(GA_type solve_type = NONE);


	//this function checks if all the variables are initialised
	bool check();

	//function wich init the GA engine
	GA* init_engine();

	//prepare parameters according to solve_type
	void prepare_parameters();

	//this method loads a benchmark coordinates data from .tsp file
	//takes a benchmark name without extention and full path and search in ./benchmarks/
	float **loadBench(std::string benchmark, string root = default_root);


	//this method loads a benchmark coordinates data from .tsp file
	//takes benchmark full path
	float **load(std::string benchmark);

	//this method calculates the distance between coordinates
	float** getWeights();

	//create the NN tour
	int* ANN(float **coords = NULL);
	
	int nth(int row, int n, bool min = true, bool index_track = true);

	static void setDefaultRoot(string root);

	void print(bool log = false);

	void setLogFile(string file);

	void setLogging(bool l);

	void redirect(bool rev = false);

	// void checkCoords();

	~TSP();
};



#endif