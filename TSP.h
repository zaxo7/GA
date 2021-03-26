#ifndef DEF_TSP
#define DEF_TSP

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "GA.h"

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

	int genomeLen;
	int codonMin;
	int codonMax;

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
	

	static void setDefaultRoot(string root);


	~TSP();
};



#endif