#include "GA_functions.h"

//random numbers generator
extern std::random_device random_device;
extern std::mt19937 *random_engine ;
extern std::uniform_int_distribution<int> *get_rand;

bool default_monitor(ga_result& state)
{
	return true;
}

genome** default_selection(genome**& population, int popSize)
{
	//allocating for the sorted pop
	/*genome** sortedPopulation = NULL;
	sortedPopulation = (genome**)malloc(sizeof(genome*) * popSize);
	if(!sortedPopulation)
	{
		//BOOST_LOG_TRIVIAL(error) << "can't allocate the sortedPopulation";
		exit(-1);
	}*/

	//copy population to sortedpop
	/*for (int i = 0; i < popSize; ++i)
	{
		sortedPopulation[i] = new genome(population[i]);
	}*/
	//memcpy(sortedPopulation, population, sizeof(genome*) * popSize);

	//sorting the population
	std::sort(population, population + popSize, gen_comp);

	//BOOST_LOG_TRIVIAL(debug) << "the sorted population";

	for (int i = 0; i < popSize; ++i)
	{
		//BOOST_LOG_TRIVIAL(debug) << *population[i];
	}


	return population;
}

int* default_crossover(int* parent1, int* parent2, int genomeLen)
{
	//BOOST_LOG_TRIVIAL(info) << "default_crossover called";
	int* child = NULL;
	child = new int[genomeLen];//(int *)malloc(sizeof(int) * genomeLen);
	if(!child)
	{
		//BOOST_LOG_TRIVIAL(error) << "can't allocate space for new child";
		exit(-1);
	}

	int cross_point = my_rand();

	//BOOST_LOG_TRIVIAL(info) << "cross_point = " << cross_point;

	if(cross_point == 0)
	{
		memcpy(child, parent2, sizeof(int) * genomeLen);
	}
	else if(cross_point == genomeLen)
	{
		memcpy(child, parent1, sizeof(int) * genomeLen);
	}
	else
	{
		memcpy(child, parent1, sizeof(int) * cross_point);

		memcpy(child + cross_point, parent2 + cross_point, sizeof(int) * (genomeLen - cross_point));
	}

	return child;
}

int default_mutation(genome *gen, float mutationChance, int *genomeMin, int *genomeMax, double dampeningFactor, bool allowRepeat)
{
	//BOOST_LOG_TRIVIAL(info) << "default_mutation called";

	
	if(!random_engine && !get_rand)
	{
		random_engine = new std::mt19937(random_device());
		get_rand = new std::uniform_int_distribution<int>(genomeMin[0], genomeMax[0]);

		random_engine->seed(time(NULL));
	}	


	int genomeLen = gen->size();

	//mutation locations
	bool mut_loc[genomeLen];

	//number of mutations to preform
	int num_mut = 0;

	//generate mutation locations acording to mutation chance
	//uniform distribution vector values in range [0 - 1]
	std::vector<double> v = stats::runif<std::vector<double>>(1,genomeLen,0,1);

	for (int i = 0; i < genomeLen; ++i)
	{
		mut_loc[i] = v[i] < mutationChance;
		if(mut_loc[i])
			num_mut++;
	}

	//go out if there is no mutations
	if(!num_mut)
		return 0;

	//get the chromosome array
	int* chromo = gen->chromo();

	//int* oldchromo = (int*)malloc(sizeof(int) * genomeLen);
	//memcpy(oldchromo, chromo, sizeof(int) * genomeLen);

	//BOOST_LOG_TRIVIAL(info) << "we gonna preform " << num_mut << " mutations on the chromosome " << *gen;
	

	#ifdef MUTATION_1
	//mutation directions
	float direction[num_mut];

	//mutation ranges
	int mutationRange[num_mut];
	
	
	std::vector<double> v2 = stats::runif<std::vector<double>>(1,num_mut,0,1);
	
	
	#endif


	//mutation new values
	int mutation[num_mut];

	int j = 0;



	for (int i = 0; i < genomeLen; ++i)
	{
		if(mut_loc[i]) //if we are in the mutation location
		{
			//BOOST_LOG_TRIVIAL(info) << "mutation number " << j << " at codon number " << i;
			
			#ifdef MUTATION_1
			
			//mutation direction
			direction[j] = v2[j] - 0.5; //to get range [-0.5 - 0.5]

			//calculate the mutation range
			mutationRange[j] = genomeMax[i] - genomeMin[i];

			//std::cout << "chromo = " << chromo[i] << " direction = " << direction[j] << " mutation Range " << mutationRange[j] << " dampeningFactor = " << dampeningFactor << " genomeMax" << genomeMax[j] << std::endl;

			//calculate the new value of codon to mutate
			mutation[j] = (int)round(chromo[i] + direction[j] * mutationRange[j] * dampeningFactor) % (genomeMax[i] + 1);
			
			#else
			
			mutation[j] =  (*get_rand)(*random_engine) % (genomeMax[i] + 1);
			
			#endif
			
			//std::cout << "rounded too " << (int)round(chromo[i] + direction[j] * mutationRange[j] * dampeningFactor) << std::endl;

			//check if the mutation is inside range
			if(mutation[j] < genomeMin[i])
				mutation[j] = genomeMin[i];
			
			//std::cout << chromo[i] << " to " << mutation[j] << std::endl;
			
			//save the old val to do a swap if we don't allow repeat
			int tmp = chromo[i];

			//apply the mutationfound
			chromo[i] = mutation[j];

			#ifdef USE_SWAP_MUTATION
			//find the duplicate and swap with it
			if(!allowRepeat)
			{
				for (int k = 0; k < genomeLen; ++k)
				{
					if(k != i && (chromo[k] == chromo[i]))
						chromo[k] = tmp;
				}
			}
			#endif
			j++;

			/*int tmp = chromo[i];
			chromo[i] = chromo[tmp % genomeLen];
			chromo[tmp % genomeLen] = tmp;*/
		}
	}

	if(!allowRepeat)
	{
		GA_h::unique_chromo(chromo, genomeMin, genomeMax, genomeLen);
	}

			//set the calculated flag back to false bcs the cost will change
	gen->rest_calculated();

	//BOOST_LOG_TRIVIAL(info) << "chromosome after mutation : " << *gen;

	/*int diff = 0;

	for(int i = 0; i < genomeLen; i++)
		if(chromo[i] != oldchromo[i])
			diff++;
	if(num_mut)
		std::cout << "number of diffrences in " << num_mut << " mutations is = " << diff << std::endl;

	free(oldchromo);*/
	return num_mut;
}



bool gen_comp(genome *&a, genome *&b)
{
	return *a < *b;
}