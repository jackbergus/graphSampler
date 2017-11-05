/*
 * SamplingAlgorithmCache.cpp
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

 

#include "SamplingAlgorithmCache.h"
#include "argsSampling.h"
#include <fstream>

SamplingAlgorithmCache::SamplingAlgorithmCache(std::string edgeFile, int samplerVertex, int samplerSkip, int samplerNext, double jumpProb)
		: jumpProb{jumpProb},
		  rj{0.0,1.0},
		  rv{0,0},
		  cache{&compareU_Int} {
	// I do not wat to cache the file position, 'cause I'm already caching all the values inside the memory
	RandomGMarkFile file{edgeFile,false};
	rv = std::uniform_int_distribution<int>(0, file.maxVertexNo());
	rvGen.seed(samplerVertex);
	rjGen.seed(samplerSkip);
	nchoiceGen.seed(samplerNext);
	counter = 0;
	u = rv(rvGen);
	addNewVertex(u);
	std::cerr << "Initializing cache" << std::endl;
	unsigned long MAX = file.edgeNo();
	for (unsigned long ii=0; ii<MAX; ii++) {
		struct vertex v{};
		file.readVertex(&v,ii);
		cache.insertKey(v.srcId)->add(false,v);
	}
}

SamplingAlgorithmCache::SamplingAlgorithmCache(argsSampling *pSampling, int maxSize)
		: jumpProb{pSampling->jumpProb},
		  rj{0.0,1.0},
		  rv{0,0},
		  cache{&compareU_Int} {
	// I do not wat to cache the file position, 'cause I'm already caching all the values inside the memory
	RandomGMarkFile file{pSampling->edgeFile,false};
	rv = std::uniform_int_distribution<int>(0, maxSize);
	rvGen.seed(pSampling->samplerVertex);
	rjGen.seed(pSampling->samplerSkip);
	nchoiceGen.seed(pSampling->samplerNext);
	counter = 0;
	u = rv(rvGen);
	addNewVertex(u);
	std::cerr << "Initializing cache" << std::endl;
	unsigned long count = 0L;
	while (file.hasNextEdge()) {
		struct vertex v{};
		if (count>1 && (count%1000000==1)) {
			std::cerr << "[RandomFileLine] Preprocessing line " << count << std::endl;
		}
		file.readNextEdge(&v, count++);
		cache.insertKey(v.srcId)->add(false,v);
	}
}


void SamplingAlgorithmCache::run(unsigned int size) {
	do {
		int v;
		int step;
		unsigned long Nusize = getVertexSize(u);
		double doJump = rj(rjGen);
		if ((Nusize>0) && doJump > jumpProb) {
			step = 0;
			std::uniform_int_distribution<> nuNext(0, (int) (Nusize - 1L));
			do {
				struct vertex vM;
				int idGen = nuNext(nchoiceGen);
				int tmp = visitEdge(u, idGen);
				if (tmp!=-1) {
					v = tmp;
					addNewVertex(v);
				}
			} while ((sampledVertexSize(v) == getVertexSize(v))&&(step++<Nusize));

			if (step <= Nusize) {
				u = v;
			} else {
				u = rv(rvGen);
				addNewVertex(u);
			}
		} else {
			u = rv(rvGen);
			addNewVertex(u);
		}
	} while (counter<size);
}

void SamplingAlgorithmCache::addNewVertex(unsigned int id) {
	Node<unsigned int, std::pair<bool, vertex>> *loc = getVertexOut(id);
	if (loc) {
		counter++;
		loc->isSampled = true;
	}
}

unsigned long SamplingAlgorithmCache::sampledVertexSize(unsigned int sampledV) {
	Node<unsigned int, std::pair<bool, vertex>> *loc = getVertexOut(sampledV);
	return loc ? loc->sampledEdgesSize : 0;
}

void SamplingAlgorithmCache::writeTo(std::string fileDest) {
	std::ofstream outputFile(fileDest+"-edges.txt");
	std::ofstream vertices(fileDest+"-vertices.txt");
	RBTree<unsigned int, std::pair<bool, vertex>>::rbiterator it = cache.iterator();
	while (it.hasNext()) {
		Node<unsigned int, std::pair<bool, vertex>> *node = it.next();
		if (node->isSampled) {
			vertices << it.getCurrentK() << std::endl;
			vertices.flush();
			for (std::pair<bool, vertex> edge : node->overflowList) {
				if (edge.first) {
					outputFile << edge.second.toString() << std::endl;
				}
			}
			outputFile.flush();
		}
	}
	vertices.close();
	outputFile.close();
}

Node<unsigned int, std::pair<bool, vertex>>* SamplingAlgorithmCache::getVertexOut(int u) {
	return cache.lookupNode(u);
}

unsigned long SamplingAlgorithmCache::getVertexSize(int u) {
	Node<unsigned int, std::pair<bool, vertex>> *loc = getVertexOut(u);
	return (!loc) ? 0L : loc->overflowList.size();
}

int SamplingAlgorithmCache::visitEdge(int u, int pos) {
	Node<unsigned int, std::pair<bool, vertex>> *loc = cache.lookupNode(u);
	if (loc) {
		loc->isSampled = true;
		if (loc->getSize()>pos) {
			if (!loc->overflowList[pos].first) {
				loc->overflowList[pos].first = true;
				loc->sampledEdgesSize++;
				return loc->overflowList[pos].second.dstId;
			}
		}
	}
	return -1;
}

void SamplingAlgorithmCache::clearAndInit(argsSampling *pSampling) {
	std::cerr << "[PreProcessing] Resetting the samples: clearing the tree…" << std::endl;
	RBTree<unsigned int, std::pair<bool, vertex>>::rbiterator it = cache.iterator();
	while (it.hasNext()) {
		Node<unsigned int, std::pair<bool, vertex>> *node = it.next();
		node->isSampled = false;
		for (unsigned int i = 0; i<node->getSize(); i++) {
			node->overflowList[i].first = false;
			node->sampledEdgesSize = 0;
		}
	}
	std::cerr << "[PreProcessing] Resetting the Seeds…"  << std::endl;
	counter = 0;
	rvGen.seed(pSampling->samplerVertex);
	rjGen.seed(pSampling->samplerSkip);
	nchoiceGen.seed(pSampling->samplerNext);
	this->jumpProb = pSampling->jumpProb;
}


