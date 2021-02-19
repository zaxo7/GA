#include "gen_functions.h"

//random numbers generator
std::random_device random_dev;
std::mt19937 *random_eng = NULL;
std::uniform_int_distribution<int> *get_rnd = NULL;

int my_rand()
{
	if(!random_eng && !get_rnd)
		init_my_rand(0, INT_MAX);
	return (*get_rnd)(*random_eng);
}

bool init_my_rand(int min, int max)
{
	if(random_eng || get_rnd)
	{
		delete random_eng;
		random_eng = NULL;
		delete get_rnd;
		get_rnd = NULL;
	}
	random_eng = new std::mt19937(random_dev());
	get_rnd = new std::uniform_int_distribution<int>(min, max);

	return true;
}