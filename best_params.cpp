//#include "best_params.h"

#include "TSP.h"

float params[64][3] = { { 50 , 100 , 0.01 },
{ 100 , 100 , 0.01 },
{ 500 , 100 , 0.01 },
{ 1000 , 100 , 0.01 },
{ 50 , 500 , 0.01 },
{ 100 , 500 , 0.01 },
{ 500 , 500 , 0.01 },
{ 1000 , 500 , 0.01 },
{ 50 , 1000 , 0.01 },
{ 100 , 1000 , 0.01 },
{ 500 , 1000 , 0.01 },
{ 1000 , 1000 , 0.01 },
{ 50 , 2000 , 0.01 },
{ 100 , 2000 , 0.01 },
{ 500 , 2000 , 0.01 },
{ 1000 , 2000 , 0.01 },
{ 50 , 100 , 0.1 },
{ 100 , 100 , 0.1 },
{ 500 , 100 , 0.1 },
{ 1000 , 100 , 0.1 },
{ 50 , 500 , 0.1 },
{ 100 , 500 , 0.1 },
{ 500 , 500 , 0.1 },
{ 1000 , 500 , 0.1 },
{ 50 , 1000 , 0.1 },
{ 100 , 1000 , 0.1 },
{ 500 , 1000 , 0.1 },
{ 1000 , 1000 , 0.1 },
{ 50 , 2000 , 0.1 },
{ 100 , 2000 , 0.1 },
{ 500 , 2000 , 0.1 },
{ 1000 , 2000 , 0.1 },
{ 50 , 100 , 0.3 },
{ 100 , 100 , 0.3 },
{ 500 , 100 , 0.3 },
{ 1000 , 100 , 0.3 },
{ 50 , 500 , 0.3 },
{ 100 , 500 , 0.3 },
{ 500 , 500 , 0.3 },
{ 1000 , 500 , 0.3 },
{ 50 , 1000 , 0.3 },
{ 100 , 1000 , 0.3 },
{ 500 , 1000 , 0.3 },
{ 1000 , 1000 , 0.3 },
{ 50 , 2000 , 0.3 },
{ 100 , 2000 , 0.3 },
{ 500 , 2000 , 0.3 },
{ 1000 , 2000 , 0.3 },
{ 50 , 100 , 0.5 },
{ 100 , 100 , 0.5 },
{ 500 , 100 , 0.5 },
{ 1000 , 100 , 0.5 },
{ 50 , 500 , 0.5 },
{ 100 , 500 , 0.5 },
{ 500 , 500 , 0.5 },
{ 1000 , 500 , 0.5 },
{ 50 , 1000 , 0.5 },
{ 100 , 1000 , 0.5 },
{ 500 , 1000 , 0.5 },
{ 1000 , 1000 , 0.5 },
{ 50 , 2000 , 0.5 },
{ 100 , 2000 , 0.5 },
{ 500 , 2000 , 0.5 },
{ 1000 , 2000 , 0.5 } };

using namespace std;

int main(int argc, char const *argv[])
{
	
	string bench;

	float small_param[2][3];
	float medium_param[2][3];
	float big_param[2][3];

	ga_result* results[64];

	TSP *solver = NULL;

	for(int i = 0; i < 3; i++)
	{
		if(i == 0) //small
		{
			cout << "finding the best parameters for small class" << endl;
			bench = "rat99";
		}
		else if(i == 1) //medium
		{
			cout << "finding the best parameters for medium class" << endl;
			bench = "lin105";	
		}
		else //big
		{
			cout << "finding the best parameters for big class" << endl;
			bench = "kroA200";
		}

		float min_cost;
		int min_cost_ind;

		cout << "solving " << bench << " SIMPLE_NN" << endl;

		int total_time = 0;

		for (int j = 0; j < 64; j++)
		{
			if(solver != NULL)
			{
				delete solver;
				solver = NULL;
			}

			solver = new TSP(bench, params[j][0], params[j][1], params[j][2]);


			results[j] = solver->solve(SIMPLE_NN);
			
			total_time += results[j]->solve_time;

			if(j == 0 || (results[j]->best_genome.cost < min_cost))
			{
				min_cost = results[j]->best_genome.cost;
				min_cost_ind = j;
			}
			cout << endl << endl << endl << 64 - j << " iterations left" << endl;

		}
		
		cout << "results for benchmark" << bench << "with SIMPLE_NN" << endl;

		if(i == 0) //small
		{
			small_param[0][0] = params[min_cost_ind][0];
			small_param[0][1] = params[min_cost_ind][1];
			small_param[0][2] = params[min_cost_ind][2];

			cout << "popsize : " << small_param[0][0] << endl
			<< "iterations : " << small_param[0][1] << endl
			<< "mutation_chance : " << small_param[0][2] << endl;
		}
		else if(i == 1) //medium
		{
			medium_param[0][0] = params[min_cost_ind][0];
			medium_param[0][1] = params[min_cost_ind][1];
			medium_param[0][2] = params[min_cost_ind][2];
			cout << "popsize : " << medium_param[0][0] << endl
			<< "iterations : " << medium_param[0][1] << endl
			<< "mutation_chance : " << medium_param[0][2] << endl;
		}
		else //big
		{
			big_param[0][0] = params[min_cost_ind][0];
			big_param[0][1] = params[min_cost_ind][1];
			big_param[0][2] = params[min_cost_ind][2];
			cout << "popsize : " << big_param[0][0] << endl
			<< "iterations : " << big_param[0][1] << endl
			<< "mutation_chance : " << big_param[0][2] << endl;
		}
		cout << "costs : " << endl;
		for (int j = 0; j < 64; ++j)
		{
			cout << results[j]->best_genome.cost << ", ";
		}

//--------------------------------------------------------------------
		cout << "solving " << bench << " SIMPLE_RND" << endl;

		total_time = 0;

		for (int j = 0; j < 64; j++)
		{
			if(solver != NULL)
			{
				delete solver;
				solver = NULL;
			}

			solver = new TSP(bench, params[j][0], params[j][1], params[j][2]);

			results[j] = solver->solve(SIMPLE_RND);

			total_time += results[j]->solve_time;

			if(j == 0 || (results[j]->best_genome.cost < min_cost))
			{
				min_cost = results[j]->best_genome.cost;
				min_cost_ind = j;
			}
			cout << endl << endl << endl << 64 - j << " iterations left" << endl;
		}

		cout << "results for benchmark" << bench << "with SIMPLE_RND" << endl;

		if(i == 0) //small
		{
			small_param[1][0] = params[min_cost_ind][0];
			small_param[1][1] = params[min_cost_ind][1];
			small_param[1][2] = params[min_cost_ind][2];

			cout << "popsize : " << small_param[1][0] << endl
			<< "iterations : " << small_param[1][1] << endl
			<< "mutation_chance : " << small_param[1][2] << endl;
		}
		else if(i == 1) //medium
		{
			medium_param[1][0] = params[min_cost_ind][0];
			medium_param[1][1] = params[min_cost_ind][1];
			medium_param[1][2] = params[min_cost_ind][2];
			cout << "popsize : " << medium_param[1][0] << endl
			<< "iterations : " << medium_param[1][1] << endl
			<< "mutation_chance : " << medium_param[1][2] << endl;
		}
		else //big
		{
			big_param[1][0] = params[min_cost_ind][0];
			big_param[1][1] = params[min_cost_ind][1];
			big_param[1][2] = params[min_cost_ind][2];
			cout << "popsize : " << big_param[1][0] << endl
			<< "iterations : " << big_param[1][1] << endl
			<< "mutation_chance : " << big_param[1][2] << endl;
		}

		cout << "costs : " << endl;
		for (int j = 0; j < 64; ++j)
		{
			cout << results[j]->best_genome.cost << ", ";
		}

//--------------------------------------------------------------------
		cout << "solving " << bench << " DOUBLE_CHROMOSOME_NN" << endl;

		total_time = 0;

		for (int j = 0; j < 64; j++)
		{
			if(solver != NULL)
			{
				delete solver;
				solver = NULL;
			}

			solver = new TSP(bench, params[j][0], params[j][1], params[j][2]);

			results[j] = solver->solve(DOUBLE_CHROMOSOME_NN);

			total_time += results[j]->solve_time;

			if(j == 0 || (results[j]->best_genome.cost < min_cost))
			{
				min_cost = results[j]->best_genome.cost;
				min_cost_ind = j;
			}
			cout << endl << endl << endl << 64 - j << " iterations left" << endl;
		}

		cout << "results for benchmark" << bench << "with DC_NN" << endl;

		if(i == 0) //small
		{
			small_param[0][0] = params[min_cost_ind][0];
			small_param[0][1] = params[min_cost_ind][1];
			small_param[0][2] = params[min_cost_ind][2];

			cout << "popsize : " << small_param[0][0] << endl
			<< "iterations : " << small_param[0][1] << endl
			<< "mutation_chance : " << small_param[0][2] << endl;
		}
		else if(i == 1) //medium
		{
			medium_param[0][0] = params[min_cost_ind][0];
			medium_param[0][1] = params[min_cost_ind][1];
			medium_param[0][2] = params[min_cost_ind][2];
			cout << "popsize : " << medium_param[0][0] << endl
			<< "iterations : " << medium_param[0][1] << endl
			<< "mutation_chance : " << medium_param[0][2] << endl;
		}
		else //big
		{
			big_param[0][0] = params[min_cost_ind][0];
			big_param[0][1] = params[min_cost_ind][1];
			big_param[0][2] = params[min_cost_ind][2];
			cout << "popsize : " << big_param[0][0] << endl
			<< "iterations : " << big_param[0][1] << endl
			<< "mutation_chance : " << big_param[0][2] << endl;
		}
		cout << "costs : " << endl;
		for (int j = 0; j < 64; ++j)
		{
			cout << results[j]->best_genome.cost << ", ";
		}

//--------------------------------------------------------------------
		cout << "solving " << bench << " DOUBLE_CHROMOSOME_RND" << endl;

		total_time = 0;

		for (int j = 0; j < 64; j++)
		{
			if(solver != NULL)
			{
				delete solver;
				solver = NULL;
			}

			solver = new TSP(bench, params[j][0], params[j][1], params[j][2]);

			results[j] = solver->solve(DOUBLE_CHROMOSOME_RND);

			total_time += results[j]->solve_time;

			if(j == 0 || (results[j]->best_genome.cost < min_cost))
			{
				min_cost = results[j]->best_genome.cost;
				min_cost_ind = j;
			}
			cout << endl << endl << endl << 64 - j << " iterations left" << endl;
		}
		cout << "results for benchmark" << bench << "with DC_RND" << endl;

		if(i == 0) //small
		{
			small_param[1][0] = params[min_cost_ind][0];
			small_param[1][1] = params[min_cost_ind][1];
			small_param[1][2] = params[min_cost_ind][2];

			cout << "popsize : " << small_param[1][0] << endl
			<< "iterations : " << small_param[1][1] << endl
			<< "mutation_chance : " << small_param[1][2] << endl;
		}
		else if(i == 1) //medium
		{
			medium_param[1][0] = params[min_cost_ind][0];
			medium_param[1][1] = params[min_cost_ind][1];
			medium_param[1][2] = params[min_cost_ind][2];
			cout << "popsize : " << medium_param[1][0] << endl
			<< "iterations : " << medium_param[1][1] << endl
			<< "mutation_chance : " << medium_param[1][2] << endl;
		}
		else //big
		{
			big_param[1][0] = params[min_cost_ind][0];
			big_param[1][1] = params[min_cost_ind][1];
			big_param[1][2] = params[min_cost_ind][2];
			cout << "popsize : " << big_param[1][0] << endl
			<< "iterations : " << big_param[1][1] << endl
			<< "mutation_chance : " << big_param[1][2] << endl;
		}

		cout << "costs : " << endl;
		for (int j = 0; j < 64; ++j)
		{
			cout << results[j]->best_genome.cost << ", ";
		}

//--------------------------------------------------------------------
		cout << "solving " << bench << " NSE_NN" << endl;

		total_time = 0;

		for (int j = 0; j < 64; j++)
		{
			if(solver != NULL)
			{
				delete solver;
				solver = NULL;
			}

			solver = new TSP(bench, params[j][0], params[j][1], params[j][2]);

			results[j] = solver->solve(NSE_NN);

			total_time += results[j]->solve_time;

			if(j == 0 || (results[j]->best_genome.cost < min_cost))
			{
				min_cost = results[j]->best_genome.cost;
				min_cost_ind = j;
			}
			cout << endl << endl << endl << 64 - j << " iterations left" << endl;
		}


		cout << "results for benchmark" << bench << "with NSE_NN" << endl;

		if(i == 0) //small
		{
			small_param[0][0] = params[min_cost_ind][0];
			small_param[0][1] = params[min_cost_ind][1];
			small_param[0][2] = params[min_cost_ind][2];

			cout << "popsize : " << small_param[0][0] << endl
			<< "iterations : " << small_param[0][1] << endl
			<< "mutation_chance : " << small_param[0][2] << endl;
		}
		else if(i == 1) //medium
		{
			medium_param[0][0] = params[min_cost_ind][0];
			medium_param[0][1] = params[min_cost_ind][1];
			medium_param[0][2] = params[min_cost_ind][2];
			cout << "popsize : " << medium_param[0][0] << endl
			<< "iterations : " << medium_param[0][1] << endl
			<< "mutation_chance : " << medium_param[0][2] << endl;
		}
		else //big
		{
			big_param[0][0] = params[min_cost_ind][0];
			big_param[0][1] = params[min_cost_ind][1];
			big_param[0][2] = params[min_cost_ind][2];
			cout << "popsize : " << big_param[0][0] << endl
			<< "iterations : " << big_param[0][1] << endl
			<< "mutation_chance : " << big_param[0][2] << endl;
		}
		cout << "costs : " << endl;
		for (int j = 0; j < 64; ++j)
		{
			cout << results[j]->best_genome.cost << ", ";
		}

//--------------------------------------------------------------------
		cout << "solving " << bench << " NSE_RND" << endl;

		total_time = 0;

		for (int j = 0; j < 64; j++)
		{
			if(solver != NULL)
			{
				delete solver;
				solver = NULL;
			}

			solver = new TSP(bench, params[j][0], params[j][1], params[j][2]);

			results[j] = solver->solve(NSE_RND);

			total_time += results[j]->solve_time;

			if(j == 0 || (results[j]->best_genome.cost < min_cost))
			{
				min_cost = results[j]->best_genome.cost;
				min_cost_ind = j;
			}
			cout << endl << endl << endl << 64 - j << " iterations left" << endl;
		}

		cout << "results for benchmark" << bench << "with NSE_RND" << endl;

		if(i == 0) //small
		{
			small_param[1][0] = params[min_cost_ind][0];
			small_param[1][1] = params[min_cost_ind][1];
			small_param[1][2] = params[min_cost_ind][2];

			cout << "popsize : " << small_param[1][0] << endl
			<< "iterations : " << small_param[1][1] << endl
			<< "mutation_chance : " << small_param[1][2] << endl;
		}
		else if(i == 1) //medium
		{
			medium_param[1][0] = params[min_cost_ind][0];
			medium_param[1][1] = params[min_cost_ind][1];
			medium_param[1][2] = params[min_cost_ind][2];
			cout << "popsize : " << medium_param[1][0] << endl
			<< "iterations : " << medium_param[1][1] << endl
			<< "mutation_chance : " << medium_param[1][2] << endl;
		}
		else //big
		{
			big_param[1][0] = params[min_cost_ind][0];
			big_param[1][1] = params[min_cost_ind][1];
			big_param[1][2] = params[min_cost_ind][2];
			cout << "popsize : " << big_param[1][0] << endl
			<< "iterations : " << big_param[1][1] << endl
			<< "mutation_chance : " << big_param[1][2] << endl;
		}
		cout << "costs : " << endl;
		for (int j = 0; j < 64; ++j)
		{
			cout << results[j]->best_genome.cost << ", ";
		}

	}

	return 0;
}