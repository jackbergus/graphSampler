/**
 * RandomGMarkFile.h
 * Created by vasistas on 04/04/17.
 */


//
// Created by Giacomo Bergami on 04/04/17.
//

#ifndef GRAPHSAMPLER_RANDOMGMARKFILE_H
#define GRAPHSAMPLER_RANDOMGMARKFILE_H

#include <string>
#include <sstream>
#include "../random/RandomFileLine.h"
#include "../future/unveil.h"

struct vertex {
	unsigned int edgeId;
	unsigned int srcId;
	unsigned int dstId;
	std::string label;
	vertex();
	vertex(const struct vertex& v);
	std::string toString();
};

class RandomGMarkFile {
	RandomFileLine rfl;

public:
	RandomGMarkFile(std::string filePath);;
	RandomGMarkFile(std::string filePath, bool cache);

	unsigned long edgeNo();
	void readVertex(struct vertex *dst, unsigned int edgeId, std::string& line);
	void readVertex(struct vertex* dst, unsigned long edgeId);

	optional<std::pair<unsigned int,unsigned int>> getVertexOut(unsigned int vertex);
	unsigned int getVertexOutgoingEdgesSize(unsigned int vertex);
	unsigned int maxVertexNo();
	std::string getVertexInFormat(unsigned int vertex);

	bool hasNextEdge();
	void readNextEdge(struct vertex *dst, unsigned long edgeId);
};


#endif //GRAPHSAMPLER_RANDOMGMARKFILE_H
