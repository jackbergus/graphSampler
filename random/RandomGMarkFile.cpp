/**
 * RandomGMarkFile.h
 * Created by vasistas on 04/04/17.
 */

//
// Created by Giacomo Bergami on 04/04/17.
//

#include <iostream>
#include "RandomGMarkFile.h"
#include "../numeric/compares.h"
#include "../printVertex.h"

RandomGMarkFile::RandomGMarkFile(std::string filePath) : RandomGMarkFile(filePath, true) {}

RandomGMarkFile::RandomGMarkFile(std::string filePath, bool cache) : rfl{filePath, cache} {}

void RandomGMarkFile::readVertex(struct vertex *dst, unsigned int edgeId, std::string& line) {
	std::stringstream sstr;
	sstr << line;
	dst->edgeId = edgeId;
	sstr >> dst->srcId;
	sstr >> dst->label;
	sstr >> dst->dstId;
}

void RandomGMarkFile::readVertex(struct vertex *dst, unsigned long edgeId) {
	std::string line = rfl.getLine(edgeId);
	std::stringstream sstr;
	sstr << line;
	dst->edgeId = edgeId;
	sstr >> dst->srcId;
	sstr >> dst->label;
	sstr >> dst->dstId;
}

void RandomGMarkFile::readNextEdge(struct vertex *dst, unsigned long edgeId) {
	std::string line = rfl.readNext();
	std::stringstream sstr;
	sstr << line;
	dst->edgeId = edgeId;
	sstr >> dst->srcId;
	sstr >> dst->label;
	sstr >> dst->dstId;
}

optional<std::pair<unsigned int, unsigned int>> RandomGMarkFile::getVertexOut(unsigned int vertex) {
	optional<std::pair<unsigned int, unsigned int>> elem;
	unsigned int i = 0;
	unsigned int max = edgeNo();
	unsigned int j = max;
	//std::cerr << "ToFound ==" << vertex << std::endl;
	while ((j<=max) && (i>=0) && j>i) {
		struct vertex vM;
		unsigned int m = (i+j)/2;
		readVertex(&vM, m);
		//printVertex(&vM);
		if (vertex == vM.srcId) {
			//std::cerr << "found @" << m << std::endl;
			unsigned int iLoc = m, iTemp = m;
			unsigned int jLoc = m, jTemp = m;
			do {
				iTemp = iLoc;
				if (iLoc == 0) break;
				readVertex(&vM, --iLoc);
			} while (vertex == vM.srcId);
			do {
				jTemp = jLoc;
				if (jLoc == max) break;
				readVertex(&vM, ++jLoc);
			} while (vertex == vM.srcId);
			elem.emplace(iTemp,jTemp);
			//std::cerr << "ptr ==" << iLoc << " <=> " << jLoc << std::endl;
			return elem;
		}
		else if (vertex < vM.srcId) {
			j = m-1;
		} else {
			i = m+1;
		}
	}
	//std::cerr << "not found" << std::endl;
	return elem;
}



unsigned long RandomGMarkFile::edgeNo() {
	unsigned long pos = rfl.getCurrentFilePos();
	rfl.setCurrentFilePos(0L);
	unsigned long val = 0;
	while (rfl.hasNext()) {
		rfl.readNext();
		val++;
	}
	rfl.setCurrentFilePos(pos);
	return val;
}

unsigned int RandomGMarkFile::maxVertexNo() {
	unsigned long pos = rfl.getCurrentFilePos();
	rfl.setCurrentFilePos(0L);
	long maxVertex = -1;
	while (rfl.hasNext()) {
		std::string line = rfl.readNext();
		struct vertex curr;
		readVertex(&curr,0,line);
		if (curr.srcId > maxVertex) {
			maxVertex = curr.srcId;
		}
	}
	rfl.setCurrentFilePos(pos);
	return (unsigned int) maxVertex;
}

unsigned int RandomGMarkFile::getVertexOutgoingEdgesSize(unsigned int vertex) {
	optional<std::pair<unsigned int, unsigned int>> res = getVertexOut(vertex);
	if (res.has_value()) {
		return res.value().second-res.value().first+1;
	} else {
		return 0;
	}
}

std::string RandomGMarkFile::getVertexInFormat(unsigned int vertex) {
	return this->rfl.getLine(vertex);
}

bool RandomGMarkFile::hasNextEdge() {
	return rfl.hasNext();
}

/// VERTEX

vertex::vertex(const struct vertex &v) {
	edgeId = v.edgeId;
	srcId = v.srcId;
	dstId = v.dstId;
	label = v.label;
}

vertex::vertex() {

}

std::string vertex::toString() {
	return std::to_string(srcId) + " " + label + " " + std::to_string(dstId);
}
