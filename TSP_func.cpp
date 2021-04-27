#include "TSP_func.h"

float** wheights;

float default_eval_simple(int* chromosome)
{
	//BOOST_LOG_TRIVIAL(info) << "evaluation function called";
	/*float cost = 0;
	for (int i = 0; i < genome::size(); ++i)
	{
		cost += chromosome[i] * (i + (i/4)) + (3 * ( i - chromosome[i]));
	}*/

	return eval_tour(chromosome);
}

float default_eval_dc(int* chromosome)
{

	return eval_tour(chromosome);
}

float default_eval_nse(int* chromosome)
{
	return eval_tour(chromosome);
}

float eval_tour(int *chromosome)
{
	BOOST_LOG_TRIVIAL(debug) << "eval_tour called";	

	float cost = 0;
  
  	int len = genome::size();

  
	for (int i = 0; i < len ; ++i)
	{
		cost += wheights[chromosome[i] - 1][chromosome[(i + 1) % len ] - 1];
	}

	return cost; 
}


void set_weights(float** w)
{
	BOOST_LOG_TRIVIAL(debug) << "set_weights called";	
	wheights = w;
}