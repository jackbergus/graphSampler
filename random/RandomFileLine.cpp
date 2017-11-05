//
// Created by Giacomo Bergami on 22/08/16.
//

#include <iostream>
#include <cmath>
#include "RandomFileLine.h"

// constructor

RandomFileLine::RandomFileLine(std::string filePath, bool vectorize) : file{filePath}, zipStream{nullptr}, vec{}, isInvector{vectorize}, errore{false} {
    std::cerr << "[RandomFileLine] " << filePath << " " << std::to_string(vectorize) << " = Counting lines" << std::endl;
    if (vectorize) {
	    unsigned long l = (unsigned long)file.tellg();
	    while (std::getline(file, line)) {
		    ++lineno;
		    if (lineno>1 && (lineno%1000000==1)) {
			    std::cerr << "[RandomFileLine] Preprocessing line " << lineno << std::endl;
		    }
		    vec.emplace_back(l);
		    l = (unsigned long)file.tellg();
	    }
	    file.clear();
	    current = 0;
	    file.seekg(std::ios::beg);
    }
}



RandomFileLine::RandomFileLine(std::string filePath) : RandomFileLine(filePath,true) {}

// methods

std::string RandomFileLine::getLine(unsigned long actualPos) {
    if (isInvector) {
        if (current>=lineno || errore) { file.clear(); errore = false;}
        file.seekg(vec[actualPos],std::ios::beg);
        if (!std::getline(file, line)) errore = true;
        current = actualPos;
        if (current>=lineno) file.clear();
        return line;
    } else {
        if (actualPos > lineno)
            actualPos = lineno;
        if (!zipStream) {
            if (current>=lineno || errore) { file.clear(); errore = false;}
            file.seekg(actualPos,std::ios::beg);
            if (!std::getline(file, line)) errore = true;
        } else {
            if (current>=lineno || errore) { zipStream->clear(); errore = false; }
            zipStream->seekg(actualPos,std::ios::beg);
            if (!std::getline(*zipStream, line)) errore = true;
        }
        current = actualPos;
        if (current>=lineno) file.clear();
        return line;
    }

}



std::string RandomFileLine::readNext() {
    if (!std::getline(file, line)) errore = true;
    current++;
    return line;
}

std::string RandomFileLine::getRandomLine(unsigned long jump) {
    return getLine(std::lround(RanHash::doub(jump)*(double)lineno));
}

bool RandomFileLine::hasNext() {
    return (current<lineno || !errore);
}

unsigned long RandomFileLine::getCurrentFilePos() {
    if (errore)  {
        if (!zipStream) {
            if (current>=lineno || errore) file.clear();
            file.seekg(std::ios::beg);
        } else {
            if (current>=lineno || errore) file.clear();
            zipStream->seekg(std::ios::beg);
        }
        errore = false;
    }
    return file.tellg();
}

std::string RandomFileLine::setCurrentFilePos(long actualPos) {
    if (!zipStream) {
        if (current>=lineno || errore) { file.clear(); errore = false;}
        file.seekg(actualPos,std::ios::beg);
        if (!std::getline(file, line)) errore = true;
    } else {
        if (current>=lineno || errore) { zipStream->clear(); errore = false; }
        zipStream->seekg(actualPos,std::ios::beg);
        if (!std::getline(*zipStream, line)) errore = true;
    }
    return line;
}


/*
RandomFileLine::RandomFileLine(ZipArchiveEntry::Ptr zipEntry) : zipStream{zipEntry->GetDecompressionStream()} {
    while (std::getline(*zipStream, line))
        ++lineno;
    zipStream->seekg(std::ios::beg);
}
*/
