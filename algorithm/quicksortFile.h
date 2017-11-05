/**
 * quicksortFile.h
 * Created by vasistas on 04/04/17.
 */


//
// Created by Giacomo Bergami on 04/04/17.
//

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
