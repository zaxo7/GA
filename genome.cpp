#include "genome.h" 

int genome::chromoSize = -1;

std::default_random_engine genome::generator;

std::uniform_int_distribution<int> *genome::distribution;

int genome::min;
int genome::max;
bool genome::allowRepeat;

eval genome::eval_func;

genome::genome(int *chromo)
{
	chromosome = NULL;
	BOOST_LOG_TRIVIAL(error) << "allocatig space for chromosome with length " << chromoSize;
	chromosome = new int[chromoSize];//(int*)malloc(sizeof(int) * chromoSize);
	if(!chromosome)
	{
		BOOST_LOG_TRIVIAL(error) << "error allocatig space for chromosome in static initialisation";
		exit(-1);
	}
	memcpy(chromosome, chromo, sizeof(int) * chromoSize);
	calculated = false;
	BOOST_LOG_TRIVIAL(debug) << "initialising genome statically to " << *this;
}

genome::genome(genome *&gen)
{
	this->chromosome = NULL;
	BOOST_LOG_TRIVIAL(error) << "allocatig space for chromosome with length " << chromoSize;
	this->chromosome = new int[chromoSize];//(int*)malloc(sizeof(int) * chromoSize);
	if(!chromosome)
	{
		BOOST_LOG_TRIVIAL(error) << "error allocatig space for chromosome in static initialisation";
		exit(-1);
	}
	memcpy(this->chromosome, gen->chromo(), sizeof(int) * chromoSize);
	cost = gen->get_cost();
	calculated = true;
}


genome::genome()
{
	chromosome = NULL;
	BOOST_LOG_TRIVIAL(debug) << "allocatig space for chromosome with length " << chromoSize;
	chromosome = new int[chromoSize]; //(int*)malloc(sizeof(int) * chromoSize);
	if(!chromosome)
	{
		BOOST_LOG_TRIVIAL(error) << "error allocatig space for chromosome in static initialisation";
		exit(-1);
	}
	calculated = false;

    if(allowRepeat)
    	for (int i = 0; i < chromoSize; ++i)
    		chromosome[i] = (*distribution)(generator);
    else
    {
    	int x = min;
    	for (int i = 0; i < chromoSize; ++i)
    	{
    		chromosome[i] = x++;
    	}
    	std::random_shuffle(chromosome, chromosome+chromoSize);
		
    }
	BOOST_LOG_TRIVIAL(debug) << "initialising genome randomlly to " << *this;
}

genome::~genome()
{
	BOOST_LOG_TRIVIAL(debug) << "distructing genome " << this << " :" << *this;
	delete[] this->chromosome;
	this->chromosome = NULL;
}

int genome::size()
{
	return chromoSize;
}

void genome::setSize(int size)
{
	BOOST_LOG_TRIVIAL(info) << "setting default genome size to " << size;
	chromoSize = size;
}

int *genome::chromo()
{
	return chromosome;
}

float genome::get_cost()
{
	if(calculated)
		return cost;
	else
	{
		cost = eval_func(this->chromo());
		calculated = true;
		return cost;
	}	
}

void genome::init_generator(int _min, int _max, bool _allowRepeat)
{	
	BOOST_LOG_TRIVIAL(info) << "initialising generator with allowRepeat = " << _allowRepeat << " min = " << _min << " max = " << _max;

	if(allowRepeat)
	{
		generator.seed(time(NULL));
		distribution = new std::uniform_int_distribution<int>(min,max);
	}

	allowRepeat = _allowRepeat;
	min = _min;
	max = _max;
}


void genome::set_eval(eval& func)
{
	BOOST_LOG_TRIVIAL(info) << "setting evaluation function";
	eval_func = func;
}

void genome::rest_calculated()
{
	calculated = false;
}

bool genome::isCalculated()
{
	return calculated;
}


std::ostream& operator<<(std::ostream& os, genome& g)
{
	for(int i = 0; i < g.chromoSize - 1; i++)
		os << g.chromosome[i] << " - " ;
	
	if(g.isCalculated())
		os << g.chromosome[g.chromoSize - 1] << " with cost = " << g.get_cost();
	else
		os << g.chromosome[g.chromoSize - 1] << " with cost = " << "NULL";

	return os;
}


bool genome::operator<(genome &a) const
{
	if(!calculated || !a.isCalculated())
		return -1;

	return this->cost < a.get_cost();
}

bool genome::operator>(genome &a) const
{
	if(!calculated || !a.isCalculated())
		return -1;

	return this->cost > a.get_cost();
}

