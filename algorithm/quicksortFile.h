/*
 * quicksortFile.h
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



#ifndef GRAPHSAMPLER_QUICKSORTFILE_H
#define GRAPHSAMPLER_QUICKSORTFILE_H


#include "../random/RandomGMarkFile.h"

class quicksortFile {
	RandomGMarkFile file;
	std::vector<unsigned long> permutation;
	std::vector<unsigned int> optimizedCopy;
	void quicksortInMemory(unsigned long p, unsigned long q);
	void quicksort(unsigned long p, unsigned long q);
	unsigned long  partition(unsigned long p, unsigned long q);
	unsigned long size;
	bool optimized;

public:
	quicksortFile(std::string fileName, bool optimized);
	void sort(std::string destination);

	unsigned long getVertexInPos(unsigned long pos);
};


#endif //GRAPHSAMPLER_QUICKSORTFILE_H
