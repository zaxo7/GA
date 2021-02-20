#include "GA_helpers.h"


//random numbers generator
std::random_device random_device;
std::mt19937 *random_engine = NULL;
std::uniform_int_distribution<int> *get_rand = NULL;

void GA_h::unique_chromo(int* chromo, int*& genMin, int*& genMax, int genomeLen)
{
	/*int *v = NULL;

	//this if need to be removed if the genMax or genMin are diffrent in some indexes
	if(!v)
	{
		v = (int*)malloc(sizeof(int) * genomeLen);
		for (int i = 1; i <= genomeLen; ++i)
		{
			v[i] = i;
		}
	}*/

	//int iv = 0;

	BOOST_LOG_TRIVIAL(info) << "unique chromo called";
	//BOOST_LOG_TRIVIAL(debug) << "before unique";
	//delete (new genome(chromo));
	//check for duplicates in the chromosome
	int ind;
	//int oldind;
	while(true)
	{
		ind = GA_h::dup(chromo, genomeLen);
		if(ind == -1)
			break;
		/*else
		{
			if(oldind != ind)
			{
				std::random_shuffle(v, v + genomeLen);
				iv = 0;
			}
			oldind = ind;
		}*/

		chromo[ind] = chromo[ind] + 1;
		//chromo[ind] = v[iv++ % genomeLen];

		if(chromo[ind] > genMax[ind])
			chromo[ind] = genMin[ind];
	}
	//BOOST_LOG_TRIVIAL(debug) << "after unique";
	//delete (new genome(chromo));
}


double* GA_h::dnorm(int min, int max)
{
  	std::vector<double> x;
  	for (int i = min; i <= max; ++i)
  	{
  		x.push_back(i);
  	}

  	std::vector<double> prb = stats::dnorm(x, 0.0, (float)max/3, false);

  	double *a = (double*)malloc(sizeof(double) * (max - min));
	
	for (int i = 0; i < max - min; ++i)
	{
		a[i] = prb[i];
	}

	return a;
}


int GA_h::dup(int *arr, int n)
{
	//BOOST_LOG_TRIVIAL(info) << "dup called";
    int duplicate = -1;
 
    // do for each element in the array
    for (int i = 0; i < n; i++)
    {
        // get value of the current element
        int val = abs(arr[i]);

        // make element at index (val-1) negative if it is positive
        if (arr[val-1] >= 0) {
            arr[val-1] = -arr[val-1];
        }
        else
        {
            // if element is already negative, it is repeated
            duplicate = val;
            break;
        }
    }
 		
 	bool found = false;
    // restore original array before returning
    for (int i = n-1; i >= 0; i--)
    {
        // make negative elements positive
        if (arr[i] < 0)
            arr[i] = -arr[i];
        if(!found && (duplicate+1) && (arr[i] == duplicate))
        {
        	duplicate = i;
        	found = true;
        }
    }
    // return duplicate element
    return duplicate;
}

static void revsort(double *a, int *ib, int n)
{

    int l, j, ir, i;
    double ra;
    int ii;

    if (n <= 1) return;

    a--; ib--;

    l = (n >> 1) + 1;
    ir = n;

    for (;;) {
  if (l > 1) {
      l = l - 1;
      ra = a[l];
      ii = ib[l];
  }
  else {
      ra = a[ir];
      ii = ib[ir];
      a[ir] = a[1];
      ib[ir] = ib[1];
      if (--ir == 1) {
    a[1] = ra;
    ib[1] = ii;
    return;
      }
  }
  i = l;
  j = l << 1;
  while (j <= ir) {
      if (j < ir && a[j] > a[j + 1]) ++j;
      if (ra > a[j]) {
    a[i] = a[j];
    ib[i] = ib[j];
    j += (i = j);
      }
      else
    j = ir + 1;
  }
  a[i] = ra;
  ib[i] = ii;
    }
}

void GA_h::ProbSampleNoReplace(int n, double *p,
                                int nans, int *ans)
{
	if(!random_engine && !get_rand)
	{
		random_engine = new std::mt19937(random_device());
		get_rand = new std::uniform_int_distribution<int>(0, INT_MAX);

		random_engine->seed(time(NULL));

		std::cout << "seeding ..." << std::endl;
	}

	//std::cout << "rand = " << (*get_rand)(*random_engine) << std::endl;

	double rT, mass, totalmass;
	int i, j, k, n1;

	//srand48(seed);

	int *perm = (int*)malloc(sizeof(int) * n);

	/* Record element identities */
	for (i = 0; i < n; i++)
		perm[i] = i;

	/* Sort probabilities into descending order */
	/* Order element identities in parallel */
	revsort(p, perm, n);

	/* Compute the sample */
	totalmass = 1;
	for (i = 0, n1 = n - 1; i < nans; i++, n1--) {
	rT = totalmass * ((float)(*get_rand)(*random_engine) / (float)RAND_MAX);
	mass = 0;
	for (j = 0; j < n1; j++) {
	  mass += p[j];
	  if (rT <= mass)
	    break;
	}
	ans[i] = perm[j];
	totalmass -= p[j];
	for (k = j; k < n1; k++) {
	  p[k] = p[k + 1];
	  perm[k] = perm[k + 1];
	}
	}

	free(perm);
}
