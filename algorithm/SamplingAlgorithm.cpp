/*
 * SamplingAlgorithm.cpp
 * This file is part of graphSamper
 *
 * Copyright (C) 2017 - Giacomo Bergami
 * Created by vasistas on 04/04/17.
 * 
 * graphSamper is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * graphSamper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with graphSamper. If not, see <http://www.gnu.org/licenses/>.
 */

 
 
#include "SamplingAlgorithm.h"
#include <fstream>

SamplingAlgorithm::SamplingAlgorithm(std::string edgeFile, int samplerVertex, int samplerSkip, int samplerNext, double jumpProb)
		: file{edgeFile},
		  jumpProb{jumpProb},
		  rj{0.0,1.0},
		  rv{0,0},
		  vertexIdToEdgeId{&compareU_Int} {

	rv = std::uniform_int_distribution<int>(0, file.maxVertexNo());
	rvGen.seed(samplerVertex);
	rjGen.seed(samplerSkip);
	nchoiceGen.seed(samplerNext);
	counter = 0;
	u = rv(rvGen);
	addNewVertex(u);
}

void SamplingAlgorithm::run(unsigned int size) {
	do {
		int v;
		int step;
		optional<std::pair<unsigned int, unsigned int>> Nu = file.getVertexOut(u);
		unsigned int Nusize = 0;
		if (Nu.has_value()) {
			Nusize = Nu.value().second-Nu.value().first+1;
		}
		//hasToJump
		double doJump = rj(rjGen);
		if ((Nusize>0) && doJump > jumpProb) {
			step = 0;

			std::uniform_int_distribution<> nuNext(0, Nusize-1);

			do {
				struct vertex vM;
				int idGen = nuNext(nchoiceGen);
				file.readVertex(&vM, Nu.value().first+idGen);
				v = vM.dstId;
				addNewVertex(v);
				addNewEdge(&vM);
			} while ((sampledVertexSize(v) == file.getVertexOutgoingEdgesSize(v))&&(step++<Nusize));

			if (step <= Nusize) {
				u = v;
				addNewVertex(u);
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

void SamplingAlgorithm::addNewVertex(unsigned int id) {
	if (!this->vertexIdToEdgeId.lookupNode(id)) {
		counter++;
		this->vertexIdToEdgeId.insertKey(id);
	}

}

void SamplingAlgorithm::addNewEdge(vertex *pVertex) {
	Node<unsigned int, unsigned int> *node = this->vertexIdToEdgeId.lookupNode(pVertex->srcId);
	if (!node->contains(pVertex->edgeId)) {
		this->vertexIdToEdgeId.lookupNode(pVertex->srcId)->add(pVertex->edgeId);
	}
}

int SamplingAlgorithm::sampledVertexSize(unsigned int sampledV) {
	Node<unsigned int, unsigned int> *node = this->vertexIdToEdgeId.lookupNode(sampledV);
	if (!node) return 0;
	return node->getSize();
}

void SamplingAlgorithm::writeTo(std::string fileDest) {
	std::ofstream outputFile(fileDest+"-edges.txt");
	std::ofstream vertices(fileDest+"-vertices.txt");
	RBTree<unsigned int, unsigned int>::rbiterator it = vertexIdToEdgeId.iterator();
	while (it.hasNext()) {
		Node<unsigned int, unsigned int> *node = it.next();
		vertices << it.getCurrentK() << std::endl;
		std::cerr << it.getCurrentK() << " Vertex K" << std::endl;
		vertices.flush();
		for (unsigned int edge : node->overflowList) {
			outputFile << file.getVertexInFormat(edge) << std::endl;
			std::cerr << "\t\t" << file.getVertexInFormat(edge) << std::endl;
		}
		outputFile.flush();
	}
	vertices.close();
	outputFile.close();
}


