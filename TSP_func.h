#ifndef TSP_F
#define TSP_F
#include "genome.h"


float default_eval_simple(int* chromosome);
float default_eval_dc(int* chromosome);
float default_eval_nse(int* chromosome);

float eval_tour(int *chromosome);

void set_weights(float** w);

void free_TSP_func_vars();

void shift_tour(int* witness, int* chromo, int len);

void swap_tour(int* witness, int* chromo, int len);

int* normalizeTour(int *tour, int len);

void set_witness_tour(int* wt, int len);

#endif