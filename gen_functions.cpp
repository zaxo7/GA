#include "gen_functions.h"

//random numbers generator
std::random_device random_dev_gen;
std::mt19937 *random_eng_gen = NULL;
std::uniform_int_distribution<int> *get_uni_rnd_gen = NULL;

int my_rand()
{
	if(!random_eng_gen && !get_uni_rnd_gen)
		init_my_rand(0, INT_MAX);
	return (*get_uni_rnd_gen)(*random_eng_gen);
}

bool init_my_rand(int min, int max)
{
	if(random_eng_gen || get_uni_rnd_gen)
	{
		delete random_eng_gen;
		random_eng_gen = NULL;
		delete get_uni_rnd_gen;
		get_uni_rnd_gen = NULL;
	}
	random_eng_gen = new std::mt19937(random_dev_gen());
	get_uni_rnd_gen = new std::uniform_int_distribution<int>(min, max);

	return true;
}

void free_genome_vars()
{
	delete random_eng_gen;
	random_eng_gen = NULL;
	delete get_uni_rnd_gen;
	get_uni_rnd_gen = NULL;
}