/**
 * quicksortFile.h
 * Created by vasistas on 04/04/17.
 */

//
// Created by Giacomo Bergami on 04/04/17.
//

#include <armadillo>
#include "quicksortFile.h"


unsigned long quicksortFile::partition(unsigned long p, unsigned long q)
{
	std::swap(permutation[p + (rand() % (q - p + 1))], permutation[q]);   // PIVOT = A[q]
	unsigned long i = p - 1;
	for(unsigned long j = p; j <= q; j++)
	{
		struct vertex vJ;
		struct vertex vQ;
		unsigned long myJ = permutation[j];
		unsigned long myQ = permutation[q];
		file.readVertex(&vJ,myJ);
		file.readVertex(&vJ,myQ);
		if(vJ.srcId <= vQ.srcId)
		{
			std::swap(permutation[++i], permutation[j]);
		}
	}
	return i;
}

unsigned long quicksortFile::getVertexInPos(unsigned long pos) {
	struct vertex v;
	file.readVertex(&v,pos);
	return v.srcId;
}

void quicksortFile::quicksort(unsigned long left, unsigned long  right) {
	unsigned long  i = left, j = right;
	unsigned long  pivotPos = (left + right) / 2;
	unsigned long pivotVertexId = optimized ? optimizedCopy[permutation[pivotPos]] : getVertexInPos(permutation[pivotPos]);
	/* partition */
	while (i <= j) {
		while ((optimized ? optimizedCopy[permutation[i]] : getVertexInPos(permutation[i])) < pivotVertexId)
			i++;
		while ((optimized ? optimizedCopy[permutation[j]] : getVertexInPos(permutation[j])) > pivotVertexId)
			j--;
		if (i <= j) {
			std::swap(permutation[i],permutation[j]);
			i++;
			j--;
		}
	};
	/* recursion */
	if (left < j)
		quicksort(left, j);
	if (i < right)
		quicksort(i, right);
}

void quicksortFile::sort(std::string destination) {
	if (optimized) {
		std::cerr << "Optimized version: [pre processing] storing values in main memory…" << std::endl;
		unsigned long MAX = file.edgeNo();
		for (unsigned long ii=0; ii<MAX; ii++) {
			struct vertex v;
			file.readVertex(&v, ii);
			this->optimizedCopy.push_back(v.srcId);
		}
	}
	std::cerr << "Sorting: [pre processing] storing values in main memory…" << std::endl;
	quicksort(0, size-1);
	this->optimizedCopy.clear();
	std::ofstream outputFile(destination);
	std::cerr << "Storing the file by reading the whole information from file…" << std::endl;
	for (unsigned long p : permutation) {
		outputFile << file.getVertexInFormat(p) << std::endl;
	}
	outputFile.close();
}

quicksortFile::quicksortFile(std::string fileName, bool optimized) : file{fileName} {
	// Creating the permutation
	std::cerr << "[Preprocessing:] preparing the permutation " << std::endl;
	unsigned long count = 0;
	size = file.edgeNo();
	for (count = 0; count < size; count++) {
		this->permutation.push_back(count);
	}
	this->optimized = optimized;
}

void quicksortFile::quicksortInMemory(unsigned long p, unsigned long q) {

}
