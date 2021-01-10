#include "GA.h"

GA::GA(int genomeLen,  int codonMin, int codonMax, int *genomeMin, int *genomeMax, int **suggestions, int popSize, int iterations, float terminationCost, float mutationChance, int elitism, monitor monitorFunc, eval evalFunc, bool allowrepeat, bool showSettings, bool verbose, char plapply)
{
	this->genomeLen = genomeLen;
	this->codonMin = codonMin;
	this->codonMax = codonMax;
	this->genomeMin = genomeMin;
	this->genomeMax = genomeMax;
	this->suggestions = suggestions;

	this->popSize = popSize;
	this->iterations = iterations;
	this->mutationChance = mutationChance;

	this->terminationCost = terminationCost;

	this->elitism = elitism;

	this->allowrepeat = allowrepeat;
	this->showSettings = showSettings;
	this->verbose = verbose;

	this->monitorFunc = monitorFunc;

	this->evalFunc = evalFunc;

	check();
}

bool GA::check()
{
	using namespace std;

	spdlog::info("checking parameters ...");

	if(genomeLen < 1)
	{
		spdlog::error("genomeLen can't be < 1");
		return false;
	}

	if (popSize < 5) 
	{
    	spdlog::error("The population size must be at least 5.");
  		return false;
  	}

  	if(iterations < 1)
	{
		spdlog::error("iterations can't be < 1");
		return false;
	}

	if(elitism > popSize)
	{
		spdlog::error("popSize must be greater than elitism");
		return false;
	}

	if(elitism < 0)
	{
		spdlog::error("elitism must be at least 0");
		return false;
	}

	if((mutationChance < 0) | (mutationChance > 1))
	{
		spdlog::error("mutationChance must be between 0 and 1");
		return false;
	}

	//check functions

	if(!monitorFunc)
	{
		spdlog::warn("no monitor function, using the default one");
		monitorFunc = default_monitor;
	}

	if(!evalFunc)
	{
		spdlog::error("no evaluation function defined, error");
		return false;
	}

	if(showSettings)
		cout << *this;

	return true;
}


ga_result GA::solve()
{






	ga_result res;
	return res;
}











std::ostream& operator<<(std::ostream& os, const GA& eng)
{
	os << "genomeLen = " << eng.genomeLen << "\ncodonMin = " << eng.codonMin << "\ncodonMax = " << eng.codonMax << "\npopSize = " << eng.popSize << "\niterations = " << eng.iterations << "\nmutationChance = " << eng.mutationChance << "\nterminationCost = " << eng.terminationCost << "\nelitism = " << eng.elitism << "\nallowRepeat = " << eng.allowrepeat << "\nverbose = " << eng.verbose << std::endl;

	return os;
}

GA::~GA()
{

}