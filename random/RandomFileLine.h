//
// Created by Giacomo Bergami on 22/08/16.
//

#pragma once

#include <string>
#include <fstream>
#include <vector>
#include "RanHash.h"

class RandomFileLine {
    unsigned long lineno = 0, current = 0;
    std::string line;
    std::ifstream file;
    std::istream* zipStream;
    bool isInvector;
    std::vector<unsigned long> vec;

public:
    RandomFileLine(std::string filePath, bool vectorize);
    RandomFileLine(std::string filePath);

    unsigned long getCurrentFilePos();
    std::string setCurrentFilePos(long actualPos);

    std::string readNext();

    bool hasNext();

    //RandomFileLine(ZipArchiveEntry::Ptr zipEntry);
    std::string getLine(unsigned long actualPos);
    std::string getRandomLine(unsigned long jump);
    bool errore;
};
