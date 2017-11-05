/*
 * RandomGMarkFile.h
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
