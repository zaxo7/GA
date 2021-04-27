#ifndef DEF_types
#define DEF_types
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp> 

#define STATS_ENABLE_INTERNAL_VEC_FEATURES
#define STATS_ENABLE_STDVEC_WRAPPERS

#include "include/stats.hpp"

#include <random>

struct genome_str
{
	int *chromosome;
	float cost;
};

//type used
struct ga_result
{
	int explored_space;
	int explored_space_before_best;
	int nbr_mutations;
	int nbr_crossovers;
	int size;
	unsigned long solve_time;
	genome_str best_genome;

	int popSize;
	int iterations;
	float mutationChance;
};

//monitor function type
typedef bool (*monitor)(ga_result& current_state);

//eval function type
typedef float (*eval)(int* chromosome);

//defined under the genome class
//typedef genome** (*selection)(genome**& population, genome**& newPopulation, int elitism);


#endif