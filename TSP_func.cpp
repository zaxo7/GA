#include "TSP_func.h"

float** weights = NULL;

int* witness_tour = NULL;
int* tmp_tour = NULL;

int witness_tour_len = -1;

int genomeLen = -1;

float default_eval_simple(int* chromosome)
{
	////BOOST_LOG_TRIVIAL(info) << "evaluation function called";
	/*float cost = 0;
	for (int i = 0; i < genome::size(); ++i)
	{
		cost += chromosome[i] * (i + (i/4)) + (3 * ( i - chromosome[i]));
	}*/

	return eval_tour(chromosome);
}

float default_eval_dc(int* chromosome)
{
	//BOOST_LOG_TRIVIAL(debug) << "default eval dc called with " << witness_tour_len << " and genome len " << genome::size() << " tab " << witness_tour;
	if(!witness_tour)
	{
		//BOOST_LOG_TRIVIAL(error) << "witness_tour not set";
		exit(-8);
	}
	if(!tmp_tour)
		tmp_tour = new int[witness_tour_len];

	memcpy(tmp_tour, witness_tour, sizeof(int) * witness_tour_len);

	swap_tour(tmp_tour, chromosome, genome::size());

	return eval_tour(tmp_tour);
}

float default_eval_nse(int* chromosome)
{	
	//BOOST_LOG_TRIVIAL(debug) << "default eval nse called with " << witness_tour_len << " and genome len " << genome::size() << " witness_tour " << witness_tour;
	if(!witness_tour)
	{
		//BOOST_LOG_TRIVIAL(error) << "witness_tour not set";
		exit(-8);
	}
	if(!tmp_tour)
		tmp_tour = new int[witness_tour_len];

	memcpy(tmp_tour, witness_tour, sizeof(int) * witness_tour_len);

	shift_tour(tmp_tour, chromosome, witness_tour_len);
	return eval_tour(tmp_tour);
}

float eval_tour(int *chromosome)
{
	//BOOST_LOG_TRIVIAL(debug) << "eval_tour called";	

	float cost = 0;
  
  	int len = witness_tour_len;

  	if(len == -1)
  	{
		//BOOST_LOG_TRIVIAL(error) << "bench size not set";	
  		exit(-9);
  	}

  
	for (int i = 0; i < len ; ++i)
	{
		cost += weights[chromosome[i] - 1][chromosome[(i + 1) % len ] - 1];
	}

	/*//BOOST_LOG_TRIVIAL(debug) << "eval cost : " << cost;
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
			//std::cout << "++weights["<<chromosome[i] - 1<<"][" << chromosome[(i + 1) % len ] - 1 << "]" << weights[chromosome[i] - 1][chromosome[(i + 1) % len ] - 1];
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
	}*/

	return cost; 
}


void set_weights(float** w)
{
	//BOOST_LOG_TRIVIAL(debug) << "set_weights called genome size = " << genome::size();	
	weights = w;
	/*weights = new float*[genome::size()];

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
	}*/
}


void free_TSP_func_vars()
{
	//BOOST_LOG_TRIVIAL(debug) << "freeing weights with size " << genome::size() << "*" << genome::size();
	/*for (int i = 0; i < genome::size(); ++i)
	{
		delete[] weights[i];
		weights[i] = NULL;
	}
	delete[] weights;
	weights = NULL;*/

	//std::cout << "deleted witness = " << witness_tour << " with len " << witness_tour_len << std::endl;
	if(witness_tour)
	{		
		for (int i = 0; i < witness_tour_len; ++i)
		{
			std::cout << witness_tour[i] << ", ";
		}
		std::cout << std::endl;
	}

	delete[] witness_tour;
	witness_tour = NULL;

	witness_tour_len = -1;

	delete[] tmp_tour;
	tmp_tour = NULL;
}


void shift_tour(int *witness, int *chromo, int len)
{
	//std::cout << "creating vrank with length of " << len;
	int vRank[len];

	for (int i = 0; i < len; ++i)
	{
		vRank[i] = i;
	}

	

	for (int i = 1; i < len; ++i)
	{
		int oldMigrantRank = vRank[i];
		int newMigrantRank = vRank[i] + chromo[i - 1];

		if(newMigrantRank >= len) newMigrantRank = newMigrantRank - len + 1;

		if(newMigrantRank > oldMigrantRank)
		{
			for (int j = 0; j < len; ++j)
				if((vRank[j] <= newMigrantRank) && (vRank[j] >= oldMigrantRank))
					vRank[j]--;
		}
		else
		{
			for (int j = 0; j < len; ++j)
				if((vRank[j] < oldMigrantRank) && (vRank[j] >= newMigrantRank))
					vRank[j]++;
		}
		vRank[i] = newMigrantRank;
	}

	int tour[len];

	for (int i = 0; i < len; ++i)
	{
		tour[i] = -55;
	}

	for (int i = 0; i < len; ++i)
	{
		tour[vRank[i]] = witness[i];
	}

	for (int i = 0; i < len; ++i)
	{
		//Rprintf("%d ", tour[i]);
		witness[i] = tour[i];
	}
}

void swap_tour(int* witness, int* chromo, int len)
{
	for (int i = 0; i < len - 1; ++i)
	{
		int tmp = witness[chromo[i]];
		witness[chromo[i]] = witness[chromo[i + 1]];
		witness[chromo[i + 1]] = tmp;
		////BOOST_LOG_TRIVIAL(debug) << "swaping witness[" << chromo[i] << "] and witness[" << chromo[i + 1] << "] with values = (" << tmp << ", " << witness[chromo[i]] << ")"; 
	}
	return;
}


int* normalizeTour(int *tour, int len)
{
	if(tour[0] == 1)
		return tour;

	int t[len];

	//find the index of city 1
	int index_1 = -1;
	for (int i = 0; i < len; ++i)
	{
		t[i] = tour[i];
		if(tour[i] == 1)
		{
			index_1 = i;
			//break;
		}
	}

	for (int i = 0; i < len; ++i)
	{
		tour[i] = t[(i + index_1) % len];
	}

	return tour;
}

void set_witness_tour(int* wt, int len)
{
	witness_tour = wt;
	witness_tour_len = len;
}