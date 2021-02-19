#ifndef DEF_GA
#define DEF_GA
#include <iostream>
#include "genome.h"
#include "GA_functions.h"
#include "GA_helpers.h"

//extern C because mylibml is compiled with gcc
/*extern "C" 
{
#include "include/libml/src/ml.h"
}*/




class GA
{
private:

	//genome propreties
	int genomeLen;
	int codonMin;
	int codonMax;
	int *genomeMin;
	int *genomeMax;

	//initial pop
	int **suggestions;
	int suggestions_count;

	//ga parameters
	int popSize;
	int iterations;
	float mutationChance;

	//termination cost
	float *terminationCost;

	//optimisation direction
	bool minimise;

	//number of elite genomes to pass without crossover or mutation
	int elitism;

	//allow codon repeat
	bool allowrepeat;


	bool showSettings;
	bool verbose;

	//random numbers generator
	std::random_device random_device;
	std::mt19937 *random_engine;
	std::uniform_int_distribution<int> *get_rand;

	//counters
	int nbr_mutations;
	int nbr_crossovers;
	unsigned long start_time;
	int explored_space_before_best;

	float *bestEvals;
	float *meanEvals;

	float best_cost;
	bool best_cost_set;

	ga_result *res;

	//functions
	//monitor function
	monitor monitorFunc;

	//evaluation
	eval evalFunc;

	//selection function 
	selection selectionFunc;

	//corossover function
	crossover crossoverFunc;

	//mutation function
	mutation mutationFunc;

	//application function (maps the chromosome to the evaluation function)


public:
	GA(	int genomeLen, 
		int codonMin,
		int codonMax,
		int *genomeMin,
		int *genomeMax,
		int **suggestions,
		int suggestions_count,
		int popSize,
		int iterations,
		float *terminationCost,
		bool minimise,
		float mutationChance,
		int elitism,
		monitor monitorFunc,
		eval evalFunc,
		selection selectionFunc,
		crossover crossoverFunc,
		mutation mutationFunc,
		bool allowrepeat,
		bool showSettings,
		bool verbose,
		char plapply);
	//check ga settings
	bool check();

	

	//init the boost logger
	void init_logger();

	//solving function
	ga_result solve();

	//init the GA variables (population ...)
	void init(genome **& population);

	//collect the results
	ga_result* collect(genome* bestGen, int iter);


	//print results
	void print();

	//print settings
	friend std::ostream& operator<<(std::ostream& os, const GA& eng);

	~GA();
};






#endif