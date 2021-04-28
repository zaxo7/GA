#include "TSP_func.h"

float** weights = NULL;

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
		cost += weights[chromosome[i] - 1][chromosome[(i + 1) % len ] - 1];
	}

	BOOST_LOG_TRIVIAL(debug) << "eval cost : " << cost;
	if(cost < 0)
	{
		std::cout << "gen size : " << genome::size() << std::endl;
		for (int i = 0; i < genome::size(); ++i)
		{
			std::cout << chromosome[i] << ", ";
		}

		for (int i = 0; i < len ; ++i)
		{
			cost += weights[chromosome[i] - 1][chromosome[(i + 1) % len ] - 1];
			std::cout << "++weights["<<chromosome[i] - 1<<"][" << chromosome[(i + 1) % len ] - 1 << "]" << weights[chromosome[i] - 1][chromosome[(i + 1) % len ] - 1];
		}

		std::cout << std::endl;
		for (int i = 0; i < genome::size(); ++i)
		{
			for (int j = 0; j < genome::size(); ++j)
			{
				std::cout << weights[i][j] << ", ";
			}
			std::cout << std::endl;
		}
		exit(-7);
	}

	return cost; 
}


void set_weights(float** w)
{
	BOOST_LOG_TRIVIAL(debug) << "set_weights called genome size = " << genome::size();	
	//weights = w;
	weights = new float*[genome::size()];

	for (int i = 0; i < genome::size(); ++i)
	{
		weights[i] = new float[genome::size()];
		for (int j = 0; j < genome::size(); ++j)
		{
			weights[i][j] = w[i][j];
			if(w[i][j] < 0)
			{
				std::cout << "w[" << i << "][" << j << "] = " << w[i][j] << std::endl;
				exit(-7);
			}
		}
		//std::cout << std::endl;
	}
}


void free_weights()
{
	BOOST_LOG_TRIVIAL(debug) << "freeing weights with size " << genome::size() << "*" << genome::size();
	for (int i = 0; i < genome::size(); ++i)
	{
		delete[] weights[i];
		weights[i] = NULL;
	}
	delete[] weights;
	weights = NULL;
}



