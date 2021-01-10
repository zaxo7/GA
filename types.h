#ifndef DEF_types
#define DEF_types
#include <spdlog/spdlog.h>

struct genome_str
{
	int *chromosome;
	float cost;
};

//type used
struct ga_result
{
	int explored_space;
	int solve_time;
	int nbr_mutations;
	int nbr_crossovers;
	int size;
	genome_str best_genome;
};

//monitor function type
typedef bool (*monitor)(ga_result current_state);

//eval function type
typedef float (*eval)(genome_str gen);


#endif