/**
 * argsSampling.h
 * Created by vasistas on 05/04/17.
 */


//
// Created by Giacomo Bergami on 05/04/17.
//

#ifndef GRAPHSAMPLER_ARGSSAMPLING_H
#define GRAPHSAMPLER_ARGSSAMPLING_H

#include <string>

class argsSampling {
public:
	std::string edgeFile = "edges.txt";
	int samplerVertex;
	int samplerSkip;
	int samplerNext;
	int sampleSize;
	double jumpProb;
	argsSampling();
	argsSampling(const argsSampling& other);
};

#endif //GRAPHSAMPLER_ARGSSAMPLING_H
