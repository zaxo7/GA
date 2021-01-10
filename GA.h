#ifndef DEF_GA
#define DEF_GA
#include <iostream>
#include "genome.h"

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

	//ga parameters
	int popSize;
	int iterations;
	float mutationChance;

	//termination cost
	float terminationCost;

	//number of elite genomes to pass without crossover or mutation
	int elitism;

	//allow codon repeat
	bool allowrepeat;


	bool showSettings;
	bool verbose;


	//functions
	//monitor function
	monitor monitorFunc;

	//evaluation
	eval evalFunc;

	//selection function

	//corossover function

	//mutation function

	//application function (maps the chromosome to the evaluation function)


public:
	GA(	int genomeLen, 
		int codonMin,
		int codonMax,
		int *genomeMin,
		int *genomeMax,
		int **suggestions,
		int popSize,
		int iterations,
		float terminationCost,
		float mutationChance,
		int elitism,
		monitor monitorFunc,
		eval evalFunc,
		bool allowrepeat,
		bool showSettings,
		bool verbose,
		char plapply);
	//check ga settings
	bool check();
	//print settings
	friend std::ostream& operator<<(std::ostream& os, const GA& eng);

	//solving function
	ga_result solve();


	~GA();
};






#endif