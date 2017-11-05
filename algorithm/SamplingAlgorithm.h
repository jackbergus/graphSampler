/**
 * SamplingAlgorithm.h
 * Created by vasistas on 04/04/17.
 */


//
// Created by Giacomo Bergami on 04/04/17.
//

#ifndef GRAPHSAMPLER_SAMPLINGALGORITHM_H
#define GRAPHSAMPLER_SAMPLINGALGORITHM_H

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

class SamplingAlgorithm {
private:
	RandomGMarkFile file;
	std::mt19937 rvGen, rjGen, nchoiceGen;
	std::uniform_int_distribution<int> rv;
	std::uniform_real_distribution<double> rj;
	double jumpProb;
	int u;
	int counter;
	RBTree<unsigned int, unsigned int> vertexIdToEdgeId{&compareU_Int};

public:
	SamplingAlgorithm(std::string edgeFile, int samplerVertex, int samplerSkip, int samplerNext, double jumpProb);
	void run(unsigned int size);
	void addNewVertex(unsigned int id);
	void addNewEdge(vertex *pVertex);
	int sampledVertexSize(unsigned int sampledV);
	void writeTo(std::string fileDest);
};


#endif //GRAPHSAMPLER_SAMPLINGALGORITHM_H
