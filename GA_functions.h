#ifndef DEF_GA_functions
#define DEF_GA_functions
#include "types.h"
#include "genome.h"
#include "GA_helpers.h"

extern "C" 
{
#include "include/libml/src/ml.h"
}

//selection function type
typedef genome** (*selection)(genome**& population, int genomeLen);

//crossover function type
typedef int* (*crossover)(int* parent1, int* parent2, int genomeLen);

//mutation function
typedef int (*mutation)(genome *gen, float mutationChance, int *genomeMin, int *genomeMax, bool allowRepeat, float dampeningFactor);


genome** default_selection(genome**& population, int popSize);

int* default_crossover(int* parent1, int* parent2, int genomeLen);

bool default_monitor(ga_result& state);

int default_mutation(genome *gen, float mutationChance, int *genomeMin, int *genomeMax, bool allowRepeat, float dampeningFactor);


bool gen_comp(genome *&a, genome *&b);


#endif