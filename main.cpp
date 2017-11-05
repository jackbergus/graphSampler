#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iterator>
#include <thread>
extern  "C" {
	#include <unistd.h>
}

#include "future/unveil.h"
#include "random/RandomGMarkFile.h"
#include "algorithm/SamplingAlgorithm.h"
#include "algorithm/quicksortFile.h"
#include "algorithm/SamplingAlgorithmCache.h"
#include "algorithm/argsSampling.h"

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}


void doSampling(argsSampling* arg) {
	SamplingAlgorithm sa{arg->edgeFile,arg->samplerVertex,arg->samplerSkip,arg->samplerNext,arg->jumpProb};
	sa.run(arg->sampleSize);
	sa.writeTo(arg->edgeFile+"-"+std::to_string(arg->sampleSize)+"-"+
	           std::to_string(arg->samplerVertex)+"-"+
	           std::to_string(arg->samplerSkip)+"-"+
	           std::to_string(arg->samplerNext)+"-"+
	           std::to_string(arg->jumpProb));
}

int main(int argc, char**argv) {
	int lineno = 0;
	char* file = "conf.txt";
	if (argc > 1) {
		file = argv[1];
	}
	std::ifstream cfgfile(file);
	std::string line;
	std::map<std::string,std::string> options;
	while( std::getline(cfgfile, line) )
	{
		std::istringstream is_line(line);
		std::string key;
		if(line.find("#") && std::getline(is_line, key, '=') )
		{
			std::string value;
			if( std::getline(is_line, value) )
				options[key] = value;
		}
	}


	std::string cmd = "sample";

	std::string edgeFile = "edges.txt";
	int samplerVertex = 0;
	int samplerSkip = 1;
	double jumpProb = 0.4;
	int maxFileInt = -1;

	std::vector<int> l{10};
	std::vector<int> next{10};


	if (options.find("cmd")!=options.end()) {
		cmd = options["cmd"];
	}
	if (options.find("opt-maxVertexNo")!=options.end()) {
		maxFileInt = std::stoi(options["opt-maxVertexNo"]);
	} else {
		if (cmd == "opt-sample") {
			std::cerr << "Error: the maximum size has to be pre-set in the optimized version" << std::endl;
			exit(1);
		}
	}
	if (options.find("file")!=options.end()) {
		edgeFile = options["file"];
	}
	if (options.find("vertex")!=options.end()) {
		samplerVertex = std::stoi(options["vertex"]);
	}
	if (options.find("skip")!=options.end()) {
		samplerSkip = std::stoi(options["skip"]);
	}
	if (options.find("next")!=options.end()) {
		next.clear();
		std::string samples = options["next"];
		{
			std::vector<std::string> splitted = split(samples,',');
			transform(splitted.begin(),splitted.end(),std::back_inserter(next),
			          [](std::string i){ int j = std::stoi(i); return j; });
		}
	}
	if (options.find("jump")!=options.end()) {
		std::istringstream i(options["jump"]);
		i >> jumpProb;
	}
	if (options.find("samples")!=options.end()) {
		l.clear();
		std::string samples = options["samples"];
		{
			std::vector<std::string> splitted = split(samples,',');
			transform(splitted.begin(),splitted.end(),std::back_inserter(l),
			          [](std::string i){ int j = std::stoi(i); return j; });
		}
	}

	if (cmd == "sort" || cmd == "opt-sort") {
		std::cout << "Sorting…" << std::endl;
		quicksortFile f{edgeFile, cmd == "opt-sort"};
		f.sort(edgeFile+"-sorted.txt");
	} else if (cmd == "sample" || cmd == "opt-sample"){
		static const unsigned long MAX = l.size() * next.size();
		unsigned long ii = 0;
		argsSampling* array = new argsSampling[MAX];
		std::thread* threads = new std::thread[MAX];
		for (int sampleSize : l) {
			for (int samplerNext : next) {
				array[ii].sampleSize = sampleSize;
				array[ii].samplerNext = samplerNext;
				array[ii].edgeFile = edgeFile;
				array[ii].jumpProb = jumpProb;
				array[ii].samplerSkip = samplerSkip;
				array[ii].samplerVertex = samplerVertex;
				ii++;
			}
		}
		if (cmd=="sample") {
			for (ii=0; ii<MAX; ii++) {
				std::cerr << "Sampling configuration: " << ii << std::endl;
				doSampling(&array[ii]);
			}
		} else {
			ii = 0L;
			if (MAX>0) {
				SamplingAlgorithmCache sac{&array[ii],maxFileInt};
				do {
					std::cerr << "[Optimized Version] Sampling configuration: " << ii << std::endl;
					sac.run(array[ii].sampleSize);
					sac.writeTo(array[ii].edgeFile+"-"+std::to_string(array[ii].sampleSize)+"-"+
					           std::to_string(array[ii].samplerVertex)+"-"+
					           std::to_string(array[ii].samplerSkip)+"-"+
					           std::to_string(array[ii].samplerNext)+"-"+
					           std::to_string(array[ii].jumpProb));
					if (ii+1<MAX) {
						sac.clearAndInit(&array[ii+1]);
					}
					ii++;
				} while (ii<MAX);
			}

		}
		/*for (ii=0; ii<MAX; ii++) {
			threads[ii] = std::thread(doSampling,&array[ii]);
		}
		for (ii=0; ii<MAX; ii++) {
			std::cout << "Waiting for" << ii << std::endl;
			threads[ii].join();
		}*/
		delete[] array;
		delete[] threads;
	}
	return 0;
}

