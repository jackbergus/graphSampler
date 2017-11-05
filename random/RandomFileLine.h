/*
 * RandomFileLine.h
 * This file is part of graphSampler
 *
 * Copyright (C) 2017 - Giacomo Bergami
 * Created by Giacomo Bergami on 22/08/16.
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
