/**
 * Created by vasistas on 04/04/17.
 */

//
// Created by Giacomo Bergami on 04/04/17.
//

#include <iostream>
#include "printVertex.h"
#include "random/RandomGMarkFile.h"

void printVertex(struct vertex *c) {
	std::cerr << "[" << c->edgeId << "] src=" << c->srcId << " edge=" << c->label << " dst=" << c->dstId << std::endl;
}
