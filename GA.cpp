#include "GA.h"




GA::GA(int genomeLen,  int codonMin, int codonMax, int *genomeMin, int *genomeMax, int **suggestions, int suggestions_count, int popSize, int iterations, float *terminationCost, bool minimise, float mutationChance, int elitism, monitor monitorFunc, eval evalFunc, selection selectionFunc, crossover crossoverFunc, mutation mutationFunc, bool allowrepeat, bool showSettings, bool verbose)
{
	this->genomeLen = genomeLen;
	this->codonMin = codonMin;
	this->codonMax = codonMax;
	this->genomeMin = genomeMin;
	this->genomeMax = genomeMax;

	this->suggestions = suggestions;
	this->suggestions_count = suggestions_count;

	this->popSize = popSize;
	this->iterations = iterations;
	this->mutationChance = mutationChance;

	this->terminationCost = terminationCost;

	this->minimise = minimise;

	this->elitism = elitism;

	this->allowrepeat = allowrepeat;
	this->showSettings = showSettings;
	this->verbose = verbose;

	this->monitorFunc = monitorFunc;

	this->evalFunc = evalFunc;

	this->selectionFunc = selectionFunc;

	this->crossoverFunc = crossoverFunc;

	this->mutationFunc = mutationFunc;

	//null

	bestEvals = NULL;
	meanEvals = NULL;


	BOOST_LOG_TRIVIAL(info) << "creating GA object";

	init_logger();

	check();
	
	std::srand(std::time(NULL));

	random_engine = new std::mt19937(random_device());
	get_rand = new std::uniform_int_distribution<int>(0, INT_MAX);

	random_engine->seed(time(NULL));

	init_my_rand(0, genomeLen);


	res = NULL;
	res = new ga_result;//(ga_result*)malloc(sizeof(ga_result));
	res->best_genome.chromosome = new int[genomeLen];

	if(!res)
	{
		BOOST_LOG_TRIVIAL(error) << "can't allocate space for result in collect function";
		exit(-1);
	}
}

//namespace logging = boost::log;

void GA::init_logger()
{
    BOOST_LOG_TRIVIAL(info) << "initialising logger";

    //logging::core::get()->set_filter(logging::trivial::severity > logging::trivial::error);
}

bool GA::check()
{
	BOOST_LOG_TRIVIAL(info) << "checking parameters ...";

	using namespace std;

	if(genomeLen < 1)
	{
		BOOST_LOG_TRIVIAL(error) << "genomeLen can't be < 1";
		return false;
	}

	if (popSize < 5) 
	{
    	BOOST_LOG_TRIVIAL(error) << "The population size must be at least 5.";
  		return false;
  	}

  	if(iterations < 1)
	{
		BOOST_LOG_TRIVIAL(error) << "iterations can't be < 1";
		return false;
	}

	if(!genomeMin)
	{
		BOOST_LOG_TRIVIAL(info) << "initialising genomeMin by default to array with genomeLen*codonMin " << genomeLen << " * " << codonMin;
		genomeMin = NULL;
		genomeMin = new int[genomeLen];//(int*)malloc(sizeof(int) * genomeLen);

		if(!genomeMin)
		{
			BOOST_LOG_TRIVIAL(error) << "cannot allocate space for genomeMin";
			exit(-1);
		}

		for (int codon = 0; codon < genomeLen; ++codon)
		{
			genomeMin[codon] = codonMin;
		}
	}

	if(!genomeMax)
	{
		BOOST_LOG_TRIVIAL(info) << "initialising genomeMax by default to array with genomeLen*codonMax " << genomeLen << " * " << codonMax;
		genomeMax = NULL;

		genomeMax = new int[genomeLen];//(int*)malloc(sizeof(int) * genomeLen);
		for (int codon = 0; codon < genomeLen; ++codon)
		{
			genomeMax[codon] = codonMax;
		}

		if(!genomeMax)
		{
			BOOST_LOG_TRIVIAL(error) << "cannot allocate space for genomeMax";
			exit(-1);
		}
	}

	if(!suggestions && (suggestions_count > 0))
	{
		BOOST_LOG_TRIVIAL(error) << "suggestions is NULL but suggestions_count > 0";
		return false;
	}

	if(suggestions && (suggestions_count < 1))
	{
		BOOST_LOG_TRIVIAL(warning) << "suggestions is given but suggestions_count < 1 so suggestions is not used";
	}

	if(elitism > popSize)
	{
		BOOST_LOG_TRIVIAL(error) << "popSize must be greater than elitism";
		return false;
	}

	if(elitism < 0)
	{
		BOOST_LOG_TRIVIAL(error) << "elitism must be at least 0";
		return false;
	}

	if((mutationChance < 0) | (mutationChance > 1))
	{
		BOOST_LOG_TRIVIAL(error) << "mutationChance must be between 0 and 1";
		return false;
	}

	//check functions

	if(!monitorFunc)
	{
		BOOST_LOG_TRIVIAL(warning) << "no monitor function given";
	}

	if(!evalFunc)
	{
		BOOST_LOG_TRIVIAL(error) << "no evaluation function defined";
		return false;
	}

	if(!selectionFunc)
	{
		BOOST_LOG_TRIVIAL(info) << "no selection function defined, using the default one";
		selectionFunc = default_selection;
	}

	if(!crossoverFunc)
	{
		BOOST_LOG_TRIVIAL(info) << "no crossover function defined, using the default one";
		crossoverFunc = default_crossover;
	}

	if(!mutationFunc)
	{
		BOOST_LOG_TRIVIAL(info) << "no mutation function defined, using the default one";
		mutationFunc = default_mutation;
	}

	if(showSettings)
		cout << *this;

	return true;
}

ga_result* GA::solve()
{
	BOOST_LOG_TRIVIAL(info) << "solving the GA";

	start_time = time(NULL);

	BOOST_LOG_TRIVIAL(info) << "started solving at " << start_time;


	///////////////////////////////////////////////////////////// INIT
	//initialisation de la population
	genome **population = NULL;
	
	init(population);

	//////////////////////////////////////////////////////////// Iterations
	if(!bestEvals)
		bestEvals = new float[iterations];//(float*)malloc(sizeof(float) * iterations);
	if(!meanEvals)
		meanEvals = new float[iterations];//(float*)malloc(sizeof(float) * iterations);
	float evalVals[popSize];

	//init counters
	nbr_mutations = 0;
	nbr_crossovers = 0;

	best_cost_set = false;

	// ga_result *result = new ga_result;//(ga_result*)malloc(sizeof(ga_result));
	// result->size = genomeLen;

	genome **newPopulation = NULL;

	newPopulation = new genome*[popSize];//(genome**)malloc(sizeof(genome*) * popSize);

	if(!newPopulation)
	{
		BOOST_LOG_TRIVIAL(error) << "can't allocate the new population";
		exit(-1);
	}

	//genome **sortedPopulation = NULL;

	//table to store 2 parents indexes for crossover 
	int *parentInd  = new int[2];//(int*)malloc(sizeof(int) * 2);

	//probability table for population to get crossover parents
	double* parentProb = NULL;
			
	//best index
	int bestInd;

	int iter;

	for(iter = 0; iter < iterations; iter++)
	{
		BOOST_LOG_TRIVIAL(info) << "starting iteration " << iter + 1;

		//std::cout << iter << std::endl;

		////////////////// Evaluation

		BOOST_LOG_TRIVIAL(info) << "calculating evaluation values";
		
		for(int chromo = 0; chromo < popSize; chromo++)
			evalVals[chromo] = population[chromo]->get_cost();

		BOOST_LOG_TRIVIAL(info) << "extracting some statistics";

		float *minEval = std::min_element(evalVals, evalVals + popSize);

		bestEvals[iter] = *minEval;
		BOOST_LOG_TRIVIAL(info) << "best cost in this iteration = " << bestEvals[iter];

		meanEvals[iter] = (std::accumulate(evalVals, evalVals + popSize, meanEvals[iter])) / popSize;
		BOOST_LOG_TRIVIAL(info) << "mean in this iteration = " << meanEvals[iter];
	
		bestInd = minEval - evalVals;

		BOOST_LOG_TRIVIAL(info) << "best cost in index = " << bestInd;
		BOOST_LOG_TRIVIAL(info) << "Done.";

		//set the best cost
		if(best_cost_set)
		{
			if(minimise && population[bestInd]->get_cost() < best_cost)
			{
				best_cost = population[bestInd]->get_cost();
				if(iter)
					explored_space_before_best = iter * popSize;
				else
					explored_space_before_best = bestInd;

			}
			else if(!minimise && population[bestInd]->get_cost() > best_cost)
			{
				best_cost = population[bestInd]->get_cost();
				if(iter)
					explored_space_before_best = iter * popSize;
				else
					explored_space_before_best = bestInd;
			}
		}
		else
		{
			best_cost = population[bestInd]->get_cost();
			best_cost_set = true;
			if(iter)
					explored_space_before_best = iter * popSize;
			else
				explored_space_before_best = bestInd;		
		}
		//sending results to monitor function
		if(monitorFunc)		
			monitorFunc(*collect(population[bestInd], iter));


		//check termination condition
		if(terminationCost && *terminationCost >= bestEvals[iter])
		{
			return collect(population[bestInd], iter);
		}

	//////////////////////////////////////////////////////////// Selection
		BOOST_LOG_TRIVIAL(info) << "applying selection";

		/*sortedPopulation = */selectionFunc(population, popSize);

		BOOST_LOG_TRIVIAL(debug) << "the elite members";

		//apply elitism
		for (int i = 0; i < elitism; ++i)
		{
			newPopulation[i] = new genome(population[i]);
			BOOST_LOG_TRIVIAL(debug) << *newPopulation[i];
		}


	//////////////////////////////////////////////////////////// Crossover
		BOOST_LOG_TRIVIAL(info) << "applying crossover";

		if(!iter)
		{

			parentProb = GA_h::dnorm(1,popSize);
		}


		//SampleReplace(10, parentProb, perm, 2, ans);
		//ProbSampleNoReplace(10, parentProb, perm, 2, ans, 122);

		//fill the rest with crossover
		for (int child = elitism; child < popSize; ++child)
		{
			//sample(popSize, parentProb, 2, &parentInd, 0, (*get_rand)(*random_engine));
			/*parentInd = (int*)malloc(sizeof(int) * 2);
			parentInd[0] = population[child]->chromo()[0];
			parentInd[1] = population[child]->chromo()[1];*/
			//BOOST_LOG_TRIVIAL(debug) << "ok here 1 " << child;
			GA_h::ProbSampleNoReplace(popSize, parentProb, 2, parentInd);
			//BOOST_LOG_TRIVIAL(debug) << "ok here 2 " << child;

			parentProb[0] = parentProb[0] - popSize - 1;
			parentProb[1] = parentProb[1] - popSize - 1;

			//std::cout << "crossover between parent " << parentInd[0] << " with cost " << population[parentInd[0]]->get_cost() << " and parent " << parentInd[1] << " with cost " << population[parentInd[1]]->get_cost() << " from the sorted population where ind 0 is " << population[0]->get_cost() << std::endl; 

			BOOST_LOG_TRIVIAL(info) << "crossover between parent " << parentInd[0] << " with cost " << population[parentInd[0]]->get_cost() << " and parent " << parentInd[1] << " with cost " << population[parentInd[1]]->get_cost() << " from the sorted population where ind 0 is " << population[0]->get_cost() << std::endl; 
			//BOOST_LOG_TRIVIAL(info) << "seed = " << seed << std::endl; 
		
			int *son = crossoverFunc((population[parentInd[0]])->chromo(), (population[parentInd[1]])->chromo(), genomeLen);


			newPopulation[child] = new genome(son);

			delete[] son;
			son = NULL;
		}
		/*free(parentProb);
		parentProb = NULL;*/

		nbr_crossovers += popSize - elitism;


		for (int i = 0; i < popSize; ++i)
		{
			//BOOST_LOG_TRIVIAL(info) << "delete=" << population[i] << " " << iter;
			delete population[i];
			population[i] = NULL;
			//sortedPopulation[i] = NULL;
		}

		memcpy(population, newPopulation, sizeof(genome*) * popSize);

		for (int i = 0; i < popSize; ++i)
		{
			//BOOST_LOG_TRIVIAL(info) << "delete=" << population[i] << " " << iter;
			newPopulation[i] = NULL;
			//sortedPopulation[i] = NULL;
		}

	//repair some chromosomes
		if(!allowrepeat)
		{
			BOOST_LOG_TRIVIAL(info) << "repairing some chromosomes ";
			for (int child = elitism; child < popSize; ++child)
			{
				GA_h::unique_chromo(population[child]->chromo(), genomeMin, genomeMax, genomeLen);
			}
		}

	//////////////////////////////////////////////////////////// Mutation

		if(mutationChance > 0)
		{
			BOOST_LOG_TRIVIAL(info) << "applying Mutation...";
			double dampening_factor;

			for (int child = elitism; child < popSize; ++child)
			{
				dampening_factor = (double)(iterations - iter) / iterations;

				nbr_mutations += mutationFunc(population[child], mutationChance, genomeMin, genomeMax, dampening_factor, allowrepeat);
				
			}
		}

		BOOST_LOG_TRIVIAL(info) << "new population";
		for (int i = 0; i < popSize; ++i)
		{
			BOOST_LOG_TRIVIAL(info) << *population[i];
		}

		//sleep(2);

		if(iter == iterations - 1)
		{
			res = collect(population[bestInd], iter);

			//delete allocated vars
			for (int i = 0; i < popSize; ++i)
			{
				//BOOST_LOG_TRIVIAL(info) << "delete=" << population[i] << " " << iter;
				delete population[i];
				population[i] = NULL;
				//sortedPopulation[i] = NULL;
			}
			delete[] population;
			population = NULL;
			delete[] newPopulation;
			newPopulation = NULL;

			delete[] parentProb;
			parentProb = NULL;

			delete[] parentInd;
			parentInd = NULL;
		}
	}

	BOOST_LOG_TRIVIAL(info) << "end of generations iteration reached";

	if(!elitism)
	{
		BOOST_LOG_TRIVIAL(info) << "recalculating evaluation values because of elitism = 0";
		
		for(int chromo = 0; chromo < popSize; chromo++)
			evalVals[chromo] = population[chromo]->get_cost();

		BOOST_LOG_TRIVIAL(info) << "extracting some statistics";

		float *minEval = std::min_element(evalVals, evalVals + popSize);

		bestEvals[iter] = *minEval;
		BOOST_LOG_TRIVIAL(info) << "best cost in this iteration = " << bestEvals[iter];
	
		bestInd = minEval - evalVals;
	}

	

	
	
	return res;
}


void GA::init(genome **& population)
{
	BOOST_LOG_TRIVIAL(info) << "initialising population ...";
	//create population
	population = new genome*[popSize];//(genome**)malloc(sizeof(genome*) * popSize);

	if(!population)
	{
		BOOST_LOG_TRIVIAL(error) << "can't allocate population";
		exit(-1);
	}

	//setting genome default size
	genome::setSize(genomeLen);

	//setting evaluation function
	genome::set_eval(evalFunc);

	//init random genomes generator
	genome::init_generator(codonMin, codonMax, allowrepeat);

	//population cursor
	int pop_cursor = 0;

	//init the population with the given suggestions or by random chromosomes
	if(suggestions && (suggestions_count > 0))
	{
		BOOST_LOG_TRIVIAL(info) << "filling initial population with the given suggestions ...";
		for(int i = 0; i < suggestions_count; i++)
		{
			population[pop_cursor++] = new genome(suggestions[i]);
		}
	}
	else
	{
		BOOST_LOG_TRIVIAL(info) << "no suggestions given, filling initial population with random genomes ...";
		for(int i = 0; i < popSize; i++)
			population[pop_cursor++] = new genome();
	}

	if(pop_cursor != popSize)
	{
		BOOST_LOG_TRIVIAL(info) << "fillingthe rest of initial population with " << popSize - pop_cursor << " random genomes ...";
		for(int i = pop_cursor; i < popSize; i++)
			population[pop_cursor++] = new genome();
	}

}


ga_result* GA::collect(genome* bestGen, int iter)
{
	res->explored_space = iter * popSize;
	res->explored_space_before_best = explored_space_before_best;
	res->nbr_mutations = nbr_mutations;
	res->nbr_crossovers = nbr_crossovers;
	res->solve_time = time(NULL) - start_time;
	res->best_genome.cost = bestGen->get_cost();
	memcpy(res->best_genome.chromosome, bestGen->chromo(), sizeof(int) * genomeLen);

	res->popSize = popSize;
	res->iterations = iterations;
	res->mutationChance = mutationChance;

	return res;
}


void GA::print()
{
	BOOST_LOG_TRIVIAL(info) << "finished with these results: ";

	using namespace std;

	cout << "popSize = " << res->popSize << endl
		<< "iterations = " << res->iterations << endl
		<< "mutationChance = " << res->mutationChance << endl
		<< "number of mutations = " << res->nbr_mutations << endl
		<< "numer of crossovers = " << res->nbr_crossovers << endl
		//<< "explored space = " << res->explored_space << " solutions" << endl
		<< "explored space until the best solution = " << res->explored_space_before_best << " solutions" << endl
		<< "solve time = " << res->solve_time << " seconds" << endl
		<< "final cost = " << res->best_genome.cost << endl
		<< "chromosome = " << endl;	

	for(int i = 0; i < genomeLen - 1; i++)
		cout << res->best_genome.chromosome[i] << " - " ;
	
	cout << res->best_genome.chromosome[genomeLen - 1] << endl;

	cout << "best evals : -------------------------------------------------" << endl;

	for (int i = 0; i < iterations; ++i)
	{
		cout << bestEvals[i] << ", ";
	}

	cout << endl << "mean evals : -------------------------------------------------" << endl;

	for (int i = 0; i < iterations; ++i)
	{
		cout << meanEvals[i] << ", ";
	}

}


std::ostream& operator<<(std::ostream& os, const GA& eng)
{
	os << "genomeLen = " << eng.genomeLen << "\ncodonMin = " << eng.codonMin << "\ncodonMax = " << eng.codonMax << "\npopSize = " << eng.popSize << "\niterations = " << eng.iterations << "\nmutationChance = " << eng.mutationChance << "\nterminationCost = " << eng.terminationCost << "\nelitism = " << eng.elitism << "\nallowRepeat = " << eng.allowrepeat << "\nverbose = " << eng.verbose << std::endl;

	return os;
}

GA::~GA()
{
	if(genomeMax)
	{
		delete[] genomeMax;
		genomeMax = NULL;
	}

	if(genomeMin)
	{
		delete[] genomeMin;
		genomeMin = NULL;
	}

	if(bestEvals)
	{
		delete[] bestEvals;
		bestEvals = NULL;
	}

	if(meanEvals)
	{
		delete[] meanEvals;
		meanEvals = NULL;
	}

	//never delete user pointers
	/*if(suggestions)
	{
		for (int i = 0; i < suggestions_count; ++i)
		{
			delete suggestions[i];
			suggestions[i] = NULL;
		}
		delete[] suggestions;
		suggestions = NULL;
	}*/

	if(terminationCost)
	{
		delete terminationCost;
		terminationCost = NULL;
	}

	delete random_engine;
	delete get_rand;

	free_genome_vars();

	GA_h::free_GA_helpers_vars();

}