#include "genome.h"

int genome::chromoSize;

eval genome::eval_func;

genome::genome(int *chromo)
{

}


genome::~genome()
{

}

int genome::size()
{
	return chromoSize;
}

void genome::setSize(int size)
{
	chromoSize = size;
}

int *genome::chromo()
{
	return chromosome;
}

float genome::get_cost()
{
	return cost;
}


void genome::set_eval(eval func)
{
	eval_func = func;
}
