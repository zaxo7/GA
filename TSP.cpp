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
    mutationFunc = NULL;

    allowrepeat = false;
    verbose = false;

    log = true;

    logFile = string("log.txt");
}

TSP::TSP(string benchmark, int popSize, int iterations, float mutationChance, bool fullPath) : TSP()
{
    BOOST_LOG_TRIVIAL(debug) << "constructor 2 called"; 
    cout << endl << "bench : " << benchmark << endl << "popSize : " << popSize << endl << "iterations : " << iterations << endl << "mutationChance : " << mutationChance << endl;
    if(fullPath)
        coordsMatrix = load(benchmark);
    else
        coordsMatrix = loadBench(benchmark);

    //calculate the arcs weights
    weightsMatrix = getWeights();

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

    prepare_parameters();

    if(!GA_engine)
    {
        delete GA_engine;
        GA_engine = NULL;
    }

    GA_engine = init_engine();

    result = GA_engine->solve();

    print(false);
    //print();
    return result;
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
    //init the weights matrix
    genome::setSize(benchSize);
    set_weights(weightsMatrix);

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

        if(evalFunc == NULL)
            evalFunc = default_eval_simple;

    }
    else if(solve_type == DOUBLE_CHROMOSOME_NN || (solve_type == DOUBLE_CHROMOSOME_RND))
    {
        allowrepeat = true;

        //int the Double chromosome type the genome len can be whatever > 2 and should be even
        genomeLen = benchSize;
        codonMin = 1;
        codonMax = benchSize;

        if(evalFunc == NULL)
            evalFunc = default_eval_dc;
    }
    else if(solve_type == NSE_NN || (solve_type == NSE_RND))
    {
        allowrepeat = true;

        //int the Node shift encoding type the genome len needs to be benchsize - 1
        genomeLen = benchSize;
        codonMin = 1;
        codonMax = benchSize;

        if(evalFunc == NULL)
            evalFunc = default_eval_nse;
    }
    if(!initial)
    {
        if(is_rand_init)
        {
            initial = new int*;//(int**)malloc(sizeof(int*));
            initial[0] = GA_h::sample(codonMin, codonMax, genomeLen, allowrepeat);
            initial_count = 1;
        }    
        else
        {
            initial = new int*;//(int**)malloc(sizeof(int*));
            initial[0] = ANN();
            initial_count = 1;
            BOOST_LOG_TRIVIAL(debug) << "initial pointer = " << initial <<" initial[0]" << initial[0]; 
        }
    }


}

float** TSP::loadBench(string benchmark, string root)
{
    BOOST_LOG_TRIVIAL(debug) << "loading bench : " << benchmark;
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
	float** coordsMat;

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
    	char *buffer = new char[fileSize];//(char*)malloc(sizeof(char) * fileSize);

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
        delete[] buffer;
        buffer = NULL;

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

        //alloc the matrix n*2 
        coordsMat = new float*[benchSize];//(float**)malloc(sizeof(float*) * benchSize);

        for(int i = 0; i < benchSize; i++)
        {
            coordsMat[i] = new float[2];//(float*)malloc(sizeof(float)*2);
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
	           	coordsMat[i][0] =  stof(matches[1]);
               	coordsMat[i++][1] =  stof(matches[2]);

	           	//BOOST_LOG_TRIVIAL(info) << "found " << matches.size() << " matches (" << coordsMat[i-1][0] << ", " << coordsMat[i-1][1] << ") in " << line ;
	        }
	        
        }
    }

	return coordsMat;
}

float** TSP::getWeights()
{
    BOOST_LOG_TRIVIAL(debug) << "getWeights called";
    
    float **weightsMat = new float*[benchSize];//(float**)malloc(sizeof(float*) * benchSize);
    
    for(int i = 0; i < benchSize; i++)
        weightsMat[i] = new float[benchSize];//(float*)malloc(sizeof(float) * benchSize);

    for (int i = 0; i < benchSize; ++i)
    {
        for (int j = 0; j < benchSize; ++j)
        {
            if(i != j)
            {
                weightsMat[i][j] = weightsMat[j][i] = round(sqrt(pow(coordsMatrix[i][0] - coordsMatrix[j][0], 2) + pow(coordsMatrix[i][1] - coordsMatrix[j][1], 2)));
            }
            else
            {
                weightsMat[i][j] = 1;
            }
        }
    }

    for (int i = 0; i < genome::size(); ++i)
    {
        std::cout << std::endl << i << " : ";
        for (int j = 0; j < genome::size(); ++j)
        {
                std::cout << (int)weightsMat[i][j] << ", ";
        }
    }

    return weightsMat;
}


int* TSP::ANN(float **coords)
{
    BOOST_LOG_TRIVIAL(debug) << "ANN called bs = " << benchSize;

    if(genomeLen == -1)
        genomeLen = benchSize;

    if(coords == NULL)
        coords = weightsMatrix;

    int* tour = new int[genomeLen];//(int*)malloc(sizeof(int) * genomeLen);
    //trouver le max dans la matrice
    int matrix_max = coords[0][0];
    for (int i = 0; i < benchSize; ++i)
    {
        for (int j = 0; j < benchSize; ++j)
        {
            if(coords[i][j] > matrix_max)
                matrix_max = coords[i][j];
        }
    }

    //remplacer la valeur de la diagonale avec le max + 100
    for (int i = 0; i < benchSize; ++i)
    {
        for (int j = 0; j < benchSize; ++j)
        {
            if(i == j)
                coords[i][j] = matrix_max + 100;
        }
    }
    /*cout << "---------------------------------------" << endl;
    for (int i = 0; i < benchSize; ++i)
    {   
        cout << endl << i << " : ";
        for (int j = 0; j < benchSize; ++j)
        {
            cout << weightsMatrix[i][j] << ", ";
        }
    }
    cout << endl;*/
    //the tour begins from city 1
    tour[0] = 1;

    //cout << nth(31, 1) << endl;

    for (int i = 1; i < benchSize; ++i)
    {
        int k = 1;
        //find the k-th minimum in row i
        int suiv = nth(tour[i - 1] - 1, k) + 1;

        //cout << "first suiv for " << tour[i - 1] - 1 << " is " << suiv << endl;

        //check if the city already exists in the tour
        bool exists = false;
        while(1)
        {
            exists = false;
            for (int j = 0; j < i; ++j)
            {
                if(tour[j] == suiv)
                    exists = true;
            }
            
            if(exists)
            {
                // cout << "\n" << suiv << " exists in " << endl;
                // for (int k = 0; k < i; ++k)
                // {
                //     cout << tour[k] << ", ";
                // }
                k++;
                suiv = nth(tour[i - 1] - 1, k) + 1;
            }
            else
            {
                // cout << "\n" << suiv << " not exists in " << endl;
                // for (int k = 0; k < i; ++k)
                // {
                //     cout << tour[k] << ", ";
                // }
                break;
            }
        }

        tour[i] = suiv;
    }

    return tour;
}


bool compare(int a, int b, float* data)
{
    return data[a]<data[b];
}

float *v;
int TSP::nth(int row, int n, bool min, bool index_track)
{
    if(!n)
        return -2;

    v = new float[benchSize];//(float*)malloc(sizeof(float) * benchSize);
    int index[benchSize];

    for (int i = 0; i < benchSize; ++i)
    {
        v[i] = weightsMatrix[row][i];
        index[i] = i;
        //printf("%f, ", weightsMatrix[row][i]);
    }

    //using namespace std::placeholders;
    
    sort(index, index + benchSize, std::bind(compare,  std::placeholders::_1, std::placeholders::_2, v));

    // for (int i = 0; i < benchSize; ++i)
    // {
    //     cout << v[i] << ", ";
    // }

    // cout << endl;

    // for (int i = 0; i < benchSize; ++i)
    // {
    //     cout << index[i] << ", ";
    // }

    // cout << endl;

    if(min)
    {

        if(index_track)
            return index[n - 1];
        else
            return v[n - 1];
    }
    else
    {
        if(index_track)
            return index[benchSize - n];
        else  
            return v[benchSize - n];
    }

    return -1;
}


void TSP::setDefaultRoot(string root)
{
    BOOST_LOG_TRIVIAL(debug) << "setting default benchmarks root to " << root; 
    default_root = root;
}


void TSP::print(bool log)
{
    int    fd;
    if(log)
    {

        fflush(stdout);
        //fgetpos(stdout, &pos);
        fd = dup(fileno(stdout));
        freopen(logFile.c_str(), "a+", stdout);
    }

    cout << "\n\n\n\nResults for " << *benchName << endl;
    GA_engine->print();

    if(log)
    {
        fflush(stdout);
        dup2(fd, fileno(stdout));
        close(fd);
    }
}

void TSP::setLogFile(string file)
{
    logFile = file;
}


void TSP::setLogging(bool l)
{
    log = l;
}














TSP::~TSP()
{
    delete benchName;
    benchName = NULL;
    delete benchPath;
    benchPath = NULL;

    delete GA_engine;
    GA_engine = NULL;

    for (int i = 0; i < benchSize; ++i)
    {
        delete[] coordsMatrix[i];
        coordsMatrix[i] = NULL;
        delete[] weightsMatrix[i];
        weightsMatrix[i] = NULL;
    }
    delete[] coordsMatrix;
    coordsMatrix = NULL;
    delete[] weightsMatrix;
    weightsMatrix = NULL;

    
    for (int i = 0; i < initial_count; ++i)
    {
        delete[] initial[i];
        initial[i] = NULL;
        //BOOST_LOG_TRIVIAL(debug) << "initial pointer = " << initial <<" initial[0]" << initial[0]; 

    }

    delete[] initial;
    initial = NULL;

    delete termination_cost;
    termination_cost = NULL;

}