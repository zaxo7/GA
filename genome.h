#ifndef DEF_GENOME
#define DEF_GENOME
#include "types.h"
#include "functions.h"


class genome
{
private:
	static int chromoSize;
	int *chromosome;
	float cost;
	bool caculated;
public:
	genome(int *chromo);
	~genome();


	//get chromosome size
	int size();

	//set the chromo size
	void setSize(int size);

	//get the chromosome array
	int *chromo();

	//get the cost & calculate it if not calculated yet
	float get_cost();

	

	static eval eval_func;

	void set_eval(eval func);
};






#endif