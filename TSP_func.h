#ifndef TSP_F
#define TSP_F
#include "genome.h"


float default_eval_simple(int* chromosome);
float default_eval_dc(int* chromosome);
float default_eval_nse(int* chromosome);

float eval_tour(int *chromosome);

void set_weights(float** w);

#endif