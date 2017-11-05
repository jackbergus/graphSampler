/*
 * argsSampling.cpp
 * This file is part of graphSampler
 *
 * Copyright (C) 2017 - Giacomo Bergami
 * Created by Giacomo Bergami on 05/04/17.
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

