#include "TSP.h"

using namespace std;

string TSP::default_root = string("./benchmarks/");

TSP::TSP()
{
    BOOST_LOG_TRIVIAL(debug) << "constructor 1 called";	
    benchName = NULL;
	benchPath = NULL;
    
    popSize = -1;
    iterations = -1;
    mutationChance = -1;

    solve_type = NONE;
    
    benchSize = -1;

    GA_engine = NULL;

    genomeLen = -1;
    codonMin = 1;
    codonMax = -1;

    initial = NULL;
    initial_count = 0;

    termination_cost = NULL;

    elitism = 1;

    monitorFunc = NULL;
    evalFunc = NULL;
    selectionFunc = NULL;
    crossoverFunc = NULL;

    allowrepeat = false;
    verbose = false;
}

TSP::TSP(string benchmark, int popSize, int iterations, float mutationChance, bool fullPath) : TSP()
{
    BOOST_LOG_TRIVIAL(debug) << "constructor 2 called"; 
    if(fullPath)
        load(benchmark);
    else
        loadBench(benchmark);

    this->popSize = popSize;
    this->iterations = iterations;
    this->mutationChance = mutationChance; 
}


TSP::TSP(string benchmark, int popSize, int iterations, float mutationChance, GA_type solve_type, bool fullPath) : TSP(benchmark, popSize, iterations, mutationChance, fullPath)
{
    BOOST_LOG_TRIVIAL(debug) << "constructor 3 called"; 
    this->solve_type = solve_type;
}

TSP::TSP(string benchmark, int popSize, int iterations, float mutationChance, GA_type solve_type, monitor monitorFunc, eval evalFunc, selection selectionFunc, crossover crossoverFunc, mutation mutationFunc, bool fullPath) : TSP(benchmark, popSize, iterations, mutationChance, solve_type, fullPath)
{
    BOOST_LOG_TRIVIAL(debug) << "constructor 4 called"; 
    this->monitorFunc = monitorFunc;
    this->evalFunc = evalFunc;
    this->selectionFunc = selectionFunc;
    this->crossoverFunc = crossoverFunc;
    this->mutationFunc = mutationFunc;
}


ga_result* TSP::solve(GA_type solve_type)
{
    if(solve_type != NONE)
        this->solve_type = solve_type;
    if(!check())
        return NULL;

    ga_result *res;

    prepare_parameters();

    if(!GA_engine)
        delete GA_engine;

    GA_engine = init_engine();

    res = GA_engine->solve();

    return res;
}

bool TSP::check()
{
    BOOST_LOG_TRIVIAL(info) << "checking TSP parameters ...";
   
    if (popSize < 5) 
    {
        BOOST_LOG_TRIVIAL(error) << "The population size must be at least 5.";
        return false;
    }

    if(iterations < 1)
    {
        BOOST_LOG_TRIVIAL(error) << "iterations can't be < 1";
        return false;
    }

    if((mutationChance < 0) | (mutationChance > 1))
    {
        BOOST_LOG_TRIVIAL(error) << "mutationChance must be between 0 and 1";
        return false;
    }

    if(benchSize < 0)
    {
        BOOST_LOG_TRIVIAL(error) << "benchmark size not valid, check the benchmark file format";
        return false;
    }

    if(solve_type == NONE)
    {
        BOOST_LOG_TRIVIAL(error) << "you must specify a solving technique for ex: NSE_NN";
        return false;
    }

    return true;
}

GA* TSP::init_engine()
{
    BOOST_LOG_TRIVIAL(debug) << "initialising engine"; 
    GA* engine;

    engine = new GA(genomeLen, codonMin, codonMax, NULL, NULL, initial, initial_count, popSize, iterations, termination_cost, true, mutationChance, elitism, monitorFunc, evalFunc, selectionFunc, crossoverFunc, mutationFunc, allowrepeat, false, verbose);

    return engine;
}

void TSP::prepare_parameters()
{
    bool is_rand_init = false;

    if(solve_type == SIMPLE_RND || (solve_type == DOUBLE_CHROMOSOME_RND) || (solve_type == NSE_RND))
        is_rand_init = true;

    if(solve_type == SIMPLE_NN || (solve_type == SIMPLE_RND))
    {
        allowrepeat = false;

        //init the parameters for simple type
        genomeLen = benchSize;
        codonMin = 1;
        codonMax = benchSize;

    }
    else if(solve_type == DOUBLE_CHROMOSOME_NN || (solve_type == DOUBLE_CHROMOSOME_RND))
    {
        allowrepeat = true;

        //int the Double chromosome type the genome len can be whatever > 2 and should be even
        genomeLen = benchSize;
        codonMin = 1;
        codonMax = benchSize;

    }
    else if(solve_type == NSE_NN || (solve_type == NSE_RND))
    {
        allowrepeat = true;

        //int the Node shift encoding type the genome len needs to be benchsize - 1
        genomeLen = benchSize;
        codonMin = 1;
        codonMax = benchSize;

    }
    if(!initial)
    {
        if(is_rand_init)
        {
            initial = (int**)malloc(sizeof(int*));
            initial[0] = GA_h::sample(codonMin, codonMax, genomeLen, allowrepeat);
            initial_count = 1;
        }    
        else
        {
            //to do create the function that find the NN tour
        }
    }


}

float** TSP::loadBench(string benchmark, string root)
{
	if(!benchName)
        benchName = new string(benchmark);

    if(!benchPath)
	   benchmark = root.append(benchmark).append(".tsp");

    BOOST_LOG_TRIVIAL(debug) << "created full path " << benchmark ;

    benchPath = new string(benchmark);

	return load(benchmark);
}


float** TSP::load(string benchmark)
{
    BOOST_LOG_TRIVIAL(debug) << "loading benchmark"; 
    //extract name
    if(!benchName)
    {
        if(!benchPath)
            benchPath = new string(benchmark);

        benchName = new string(benchmark);

        //get what after the last slash
        int index = benchName->find_last_of("\\/");
        
        if(index != -1)
            benchName->erase(0, index + 1);

        index = benchName->rfind(".");

        if(index != -1)
            benchName->erase(index);

        BOOST_LOG_TRIVIAL(debug) << "got file name " << *benchName << " from " << *benchPath;
    }

	//coordinates matrix
	float** coordsMatrix;

    //benchmark file
    ifstream benchFile;

    benchFile.open(benchmark);

    if(!benchFile.good())
    {
        BOOST_LOG_TRIVIAL(error) << "benchmark file " << benchmark << " dosn't exist";
        exit(-1);
    }
    else
    {
    	//get file size
    	benchFile.seekg(0, ios::end);

    	//get cursor position
    	int fileSize = benchFile.tellg();

    	//reinit the cursor
    	benchFile.clear();
    	benchFile.seekg(0);

    	//create the buffer
    	char *buffer = (char*)malloc(sizeof(char) * fileSize);

    	//read all the file
        benchFile.read(buffer, fileSize);

        //close the file
        benchFile.close();

        BOOST_LOG_TRIVIAL(debug) << "benchmark file " << benchmark << " with size " << fileSize ;

        //regex to match bench size
        regex regBenchSize("N\\s*:\\s*(\\d+)", regex_constants::ECMAScript);

        //matches array
        smatch matches;

        //convert buffer to string
        string text = string(buffer);
        free(buffer);

        if(regex_search(text, matches, regBenchSize))
        {
        	//convert it to int
           	benchSize = stoi(matches[1]);

           	BOOST_LOG_TRIVIAL(info) << "found benchmark size " << benchSize << " in " << matches[1];
        }
        else
        {
        	BOOST_LOG_TRIVIAL(error) << "can't parse benchmark size from file " << benchmark;
        	//exit imidiately
        	exit(-1);
        }

        //alloc the matrix n*n 
        coordsMatrix = (float**)malloc(sizeof(float*) * benchSize);

        for(int i = 0; i < benchSize; i++)
        {
            coordsMatrix[i] = (float*)malloc(sizeof(int)*2);
        }

  
        regex regcoordlines("\\d+\\s+(-?\\d+(?:\\.\\d+)?(?:e[\\+\\-]\\d+)?)\\s+(-?\\d+(?:\\.\\d+)?(?:e[\\+\\-]\\d+)?)", regex_constants::ECMAScript);

        stringstream stream(text);

        string line;

        int i = 0;

        while(getline(stream, line, '\n'))
        {  	
	        regex_search(line, matches, regcoordlines);
	        
	        if(matches.size() == 3)
	        {	
	        	//convert it to float   	
	           	coordsMatrix[i][0] =  stof(matches[1]);
               	coordsMatrix[i++][1] =  stof(matches[2]);

	           	//BOOST_LOG_TRIVIAL(info) << "found " << matches.size() << " matches (" << coordsMatrix[i-1][0] << ", " << coordsMatrix[i-1][1] << ") in " << line ;
	        }
	        
        }
    }

	return coordsMatrix;
}


void TSP::setDefaultRoot(string root)
{
    BOOST_LOG_TRIVIAL(debug) << "setting default benchmarks root to " << root; 
    default_root = root;
}
















TSP::~TSP()
{

}