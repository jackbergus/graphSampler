/*
 * SamplingAlgorithm.h
 * This file is part of graphSampler
 *
 * Copyright (C) 2017 - Giacomo Bergami
 * Created by Giacomo Bergami on 04/04/17.
 * 
 * graphSampler is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * graphSampler is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with graphSampler. If not, see <http://www.gnu.org/licenses/>.
 */


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
