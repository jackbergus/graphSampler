/**
 * SamplingAlgorithmCache.h
 * Created by vasistas on 04/04/17.
 */


//
// Created by Giacomo Bergami on 04/04/17.
//

#ifndef GRAPHSAMPLER_SamplingAlgorithmCache_H
#define GRAPHSAMPLER_SamplingAlgorithmCache_H

#include <map>
#include <vector>
#include <set>
#include <string>
#include <random>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>
#include "../random/RandomGMarkFile.h"
#include "../numeric/compares.h"
#include "../rbtree/rbtree.h"
#include "argsSampling.h"

class SamplingAlgorithmCache {
private:
	std::mt19937 rvGen, rjGen, nchoiceGen;
	std::uniform_int_distribution<int> rv;
	std::uniform_real_distribution<double> rj;
	double jumpProb;
	int u;
	int counter;
	RBTree<unsigned int, std::pair<bool,struct vertex>> cache{&compareU_Int};

public:
	SamplingAlgorithmCache(std::string edgeFile, int samplerVertex, int samplerSkip, int samplerNext, double jumpProb);
	SamplingAlgorithmCache(argsSampling *pSampling,int maxSize);

	void clearAndInit(argsSampling *pSampling);
	void run(unsigned int size);
	void addNewVertex(unsigned int id);
	unsigned long sampledVertexSize(unsigned int sampledV);
	void writeTo(std::string fileDest);

	Node<unsigned int, std::pair<bool, vertex>> * getVertexOut(int u);

	int visitEdge(int u, int pos);
	unsigned long getVertexSize(int u);
};


#endif //GRAPHSAMPLER_SamplingAlgorithmCache_H
