#include "TSP.h"

string benchmarks[] = {"eil51","berlin52","st70","eil76","rat99","kroB100","kroA100","rd100","eil101","lin105","ch130","ch150","d198","kroA200"};

float simp_small[2][3] = {
	{100,2000,0.01},
	{100,2000,0.01}
};
float dc_small[2][3] = {
	{1000,2000,0.01},
	{1000,1000,0.01}
};
float nse_small[2][3] = {
	{1000,2000,0.01},
	{1000,2000,0.01}
};

float simp_med[2][3] = {
	{1000,2000,0.01},
	{500,2000,0.01}
};
float dc_med[2][3] = {
	{1000,2000,0.01},
	{500,2000,0.01}
};
float nse_med[2][3] = {
	{1000,2000,0.01},
	{1000,1000,0.01}
};

float simp_big[2][3] = {
	{1000,2000,0.01},
	{1000,1000,0.01}
};
float dc_big[2][3] = {
	{1000,2000,0.01},
	{500,2000,0.01}
};
float nse_big[2][3] = {
	{1000,2000,0.01},
	{1000,1000,0.01}
};

using namespace std;

void mat_cp(float **dst, float src[2][3], int l, int c);

int main(int argc, char const *argv[])
{
	int bench_initial = 0;
	int variant_initial = 0;
	int stop_counter = -1;
	//charger les arguments
	if(argc > 1)
	{
		//le benchmark
		bench_initial = atoi(argv[1]);
		if(bench_initial < 0 || (bench_initial >= 14))
			bench_initial = 0;
			// cout << bench_initial << endl;
		if(argc > 2)
		{
			variant_initial = atoi(argv[2]);
			if(variant_initial < 0 || (variant_initial >= 6))
				variant_initial = 0;
			// cout << variant_initial << endl;
			if(argc > 3)
			{
				stop_counter = atoi(argv[3]);
				if(stop_counter <= 0)
					stop_counter = -1;
			}
		}
	}
	//parameters matrix
	float **SIMP_param = new float*[2];
	float **DC_param =  new float*[2];
	float **NSE_param =  new float*[2];
	for (int i = 0; i < 2; ++i)
	{
		SIMP_param[i] = new float[3];
		DC_param[i] = new float[3];
		NSE_param[i] = new float[3];
	}

	int global_time = 0;

	//solver object
	TSP *solver = NULL;

	//iterate threw the 14 benchmarks
	for (int bench = bench_initial; bench < 14; ++bench)
	{
		if(stop_counter != -1)
		{
			if(stop_counter >= 1)
				stop_counter--;
			else
				exit(8);
		}
		if(bench < 5)
		{
			cout << "\n\n\nsolving benchmark " << benchmarks[bench] << " in the small class" << endl;
			//SIMP_param = (float**)simp_small;
			//memcpy(SIMP_param, &simp_small[0][0], 2 * 3 * sizeof(float));
			mat_cp(SIMP_param, simp_small, 2, 3);
			// DC_param = (float**)dc_small;
			// memcpy(DC_param, dc_small, 2 * 3 * sizeof(float));
			mat_cp(DC_param, dc_small, 2, 3);
			// NSE_param = (float**)nse_small;
			// memcpy(NSE_param, nse_small, 2 * 3 * sizeof(float));
			mat_cp(NSE_param, nse_small, 2, 3);

		}
		else if(bench >= 5 && (bench < 10))
		{
			cout << "\n\n\nsolving benchmark " << benchmarks[bench] << " in the medium class" << endl;
			// SIMP_param = (float**)simp_med;
			mat_cp(SIMP_param, simp_med, 2, 3);
			// DC_param = (float**)dc_med;
			mat_cp(DC_param, dc_med, 2, 3);
			// NSE_param = (float**)nse_med;
			mat_cp(NSE_param, nse_med, 2, 3);
		}
		else
		{
			cout << "\n\n\nsolving benchmark " << benchmarks[bench] << " in the big class" << endl;
			// SIMP_param = (float**)simp_med;
			mat_cp(SIMP_param, simp_big, 2, 3);
			// DC_param = (float**)dc_med;
			mat_cp(DC_param, dc_big, 2, 3);
			// NSE_param = (float**)nse_med;
			mat_cp(NSE_param, nse_big, 2, 3);
		}

		//results array
		ga_result* results[30];

		//execute each benchmark 30 times with 6 variants
		//simple_NN ------------------------------------------------------------------------

		GA_type solve_type;

		string type_name;

		float param[3];

		float min_cost;
		int min_cost_ind;
		
		int total_time = 0;

		for(int var = variant_initial; var < 6; var++)
		{
			if(var == 0)
			{
				solve_type = SIMPLE_NN;
				type_name = "SIMPLE_NN";
				for (int i = 0; i < 3; ++i)
				{
					param[i] = SIMP_param[0][i];
				}
			}
			else if(var == 1)
			{
				solve_type = SIMPLE_RND;
				type_name = "SIMPLE_RND";
				for (int i = 0; i < 3; ++i)
				{
					param[i] = SIMP_param[1][i];
				}
			}
			else if(var == 2)
			{
				solve_type = DOUBLE_CHROMOSOME_NN;
				type_name = "DOUBLE_CHROMOSOME_NN";
				for (int i = 0; i < 3; ++i)
				{
					param[i] = DC_param[0][i];
				}
			}
			else if(var == 3)
			{
				solve_type = DOUBLE_CHROMOSOME_RND;
				type_name = "DOUBLE_CHROMOSOME_RND";
				for (int i = 0; i < 3; ++i)
				{
					param[i] = DC_param[1][i];
				}
			}
			else if(var == 4)
			{
				solve_type = NSE_NN;
				type_name = "NSE_NN";
				for (int i = 0; i < 3; ++i)
				{
					param[i] = NSE_param[0][i];
				}
			}
			else if(var == 5)
			{
				solve_type = NSE_RND;
				type_name = "NSE_RND";
				for (int i = 0; i < 3; ++i)
				{
					param[i] = NSE_param[1][i];
				}
			}

			global_time += total_time;
			cout << "\n\n\nglobal time : " << total_time/60 << " min -- " << total_time/60/60 << " hours\n\n\n" << endl;
			total_time = 0;
			
			cout << "solving " << benchmarks[bench] << " with " << type_name << endl;

			for (int i = 0; i < 30; ++i)
			{
				if(solver)
				{
					delete solver;
					solver = NULL;
				}

				solver = new TSP(benchmarks[bench], param[0], param[1], param[2]);

				results[i] = solver->solve(solve_type);
				
				total_time += results[i]->solve_time;

				if(i == 0 || (results[i]->best_genome.cost < min_cost))
				{
					min_cost = results[i]->best_genome.cost;
					min_cost_ind = i;
				}

				cout << endl << endl << endl << 29 - i << " iterations left" << endl << "time spent : " << total_time << " s -- " << total_time/60 << " min" << endl << "time left : " << (total_time/(i+1)) * (29 - i) << " s -- " << ((total_time/(i+1)) * (29 - i)) / 60 << " min" << endl << "time for iteration : " << total_time/(i+1) << " s -- " << total_time/(i+1)/60 << " min" << endl;
			}

			cout << "final results for 30 exec of benchmark " << benchmarks[bench] << " with " << type_name << " :" << endl;
			cout << "total solve time for 30 exec : " << total_time/60 << " min -- " << total_time/60/60 << " hours" << endl;
			cout << "found best solution in iteration : " << min_cost_ind+1 << " >> " << results[min_cost_ind]->best_genome.cost << endl;
			cout << "costs : " << endl;
			for (int i = 0; i < 30; i++)
			{
				cout << results[i]->best_genome.cost << ", ";
				
				//free to avoid memory leak
				// delete[] results[j]->best_genome.chromosome;
				// delete results[i];
			}

			cout << endl;

		}
		variant_initial = 0;
	}


	return 0;
}

void mat_cp(float **dst, float src[2][3], int l, int c)
{
	for (int i = 0; i < l; ++i)
	{
		for (int j = 0; j < c; ++j)
		{
			dst[i][j] = src[i][j];
		}
	}
}