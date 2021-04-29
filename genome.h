#ifndef DEF_GENOME
#define DEF_GENOME
#include "types.h"
#include "gen_functions.h"
#include <iostream>

#include <random>


class genome
{
private:
	static int chromoSize;
	int *chromosome;
	float cost;
	bool calculated;


	static int min;
	static int max;
	static bool allowRepeat;

	static std::default_random_engine generator;
	static std::uniform_int_distribution<int> *distribution;


public:
	genome(int *chromo);
	genome(genome *&gen);
	genome();
	~genome();


	//get chromosome size
	static int size();

	//set the chromo size
	static void setSize(int size);

	//get the chromosome array
	int *chromo();

	//get the cost & calculate it if not calculated yet
	float get_cost();

	//initialise the randomness generator
	static void init_generator(int _min, int _max, bool _allowRepeat);
	
	//free allocated variables
	static void free_genome_vars();

	static eval eval_func;

	static void set_eval(eval& func);

	void rest_calculated(void);

	bool isCalculated();

	friend std::ostream& operator<<(std::ostream& os, genome& eng);

	bool operator<(genome &a) const;
  
	bool operator>(genome &a) const;

};


#endif