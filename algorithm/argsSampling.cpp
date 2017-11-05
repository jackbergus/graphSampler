/**
 * argsSampling.h
 * Created by vasistas on 05/04/17.
 */

//
// Created by Giacomo Bergami on 05/04/17.
//

#include "argsSampling.h"

argsSampling::argsSampling() {
	sampleSize = 0;
	samplerSkip = 1;
	samplerNext = 2;
	sampleSize = 0;
	jumpProb = 0.4;
}

argsSampling::argsSampling(const argsSampling &other) {
	this->edgeFile = other.edgeFile;
	this->jumpProb = other.jumpProb;
	this->samplerVertex = other.samplerVertex;
	this->samplerSkip = other.samplerSkip;
	this->samplerNext = other.samplerNext;
	this->sampleSize = other.sampleSize;
}

