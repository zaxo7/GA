#ifndef DEF_GA_helpers
#define DEF_GA_helpers

#include <iostream>
#include "types.h"
#include "genome.h"

//ga helpers namespace
namespace GA_h
{
	//unique chromosome repair
	void unique_chromo(int* gen, int*& genMin, int*& genMax, int genomeLen);

	//normal ditribution function
	double* dnorm(int min, int max);

	//check for duplicates
	int dup(int *arr, int n);

	void ProbSampleNoReplace(int n, double *po,
                                int nans, int *ans);

	int *sample(int k, int n, bool replace);
	
	int *sample(int min_k, int k, int n, bool replace);
}


#endif