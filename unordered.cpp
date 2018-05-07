/*
 * unordered.cpp
 * This file is part of graphSampler
 *
 * Copyright (C) 2018 - Giacomo Bergami
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


#include <boost/dynamic_bitset.hpp>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>
#include <stack>
#include <unordered_set>
#include <bits/unordered_set.h>
#include <queue>

template<typename Out> void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

int main(int argc, char** argv) {

	int lineno = 0;
	char* file = "conf.txt";
	if (argc > 1) {
		file = argv[1];
	}
	std::ifstream cfgfile(file);
	std::unordered_map<std::string,std::string> options;
    if (cfgfile.good()) {
		std::string line;

		while( std::getline(cfgfile, line) ){
			std::istringstream is_line(line);
			std::string key;
			if(line.find("#") && std::getline(is_line, key, '=') )
			{
				std::string value;
				if( std::getline(is_line, value) )
					options[key] = value;
			}
		}
	} else {
        std::cerr << "Error: configuration file '" << file << "' does not exists. I'm going to use the default settings " << std::endl;
    }

	std::string graph{"graph.txt"};
	if (options.find("graph")!=options.end()) {
		graph = options["graph"];
	}

	std::string line;
	std::ifstream infile(graph);
	std::unordered_map<unsigned long, std::vector<std::pair<bool,unsigned long>>> map;
	unsigned long vCount = 0;
	unsigned long count = 0;

    if (infile.good()) {
		std::cout << "Loading the graph file (" << graph << ")..." << std::endl;
        while (std::getline(infile, line)) {
            if (line.find("#") != 0) {
                unsigned long src, dst;
                std::istringstream iss(line);
                std::string token;
                int c = 0;
                while(std::getline(iss, token, '\t')) {
                    if (c++)
                        dst = std::stoul (token,nullptr,0);
                    else
                        src = std::stoul (token,nullptr,0);
                }
                map[src].emplace_back(false,dst);
                count++;
                if (count % 100000 == 0)
                    std::cout << count << std::endl;
                vCount = std::max(src,vCount);
                vCount = std::max(dst,vCount);
            }
        }
		std::cout << "Adjacency matrix loaded in main memory" << std::endl;
    } else {
        std::cerr << "Error: the adjacency list file '" << graph << "' does not exists. I'm going to use the default settings " << std::endl;
    }

    const std::vector<unsigned long>::value_type init = 0;
	std::vector<unsigned long> adjSCount;
    for (unsigned long i = 0; i<=vCount; i++) {
        adjSCount.emplace_back(0);
    }
	std::vector<unsigned long> l{10};
	if (options.find("samples")!=options.end()) {
		l.clear();
		std::string samples = options["samples"];
		{
			std::vector<std::string> splitted = split(samples,',');
			transform(splitted.begin(),splitted.end(),std::back_inserter(l),
			          [](std::string i){ unsigned long j = std::stoul(i); return j; });
		}
		std::sort(l.begin(), l.end());
	}

	// Selecting the next vertex step
	std::mt19937 rvGen;
	int samplerVertex = 0;
	if (options.find("vertex")!=options.end()) {
		samplerVertex = std::stoi(options["vertex"]);
	}
	rvGen.seed(samplerVertex);

	// Skipping to the next vertex
	std::mt19937 rjGen;
	int samplerSkip = 1;
	if (options.find("skip")!=options.end()) {
		samplerSkip = std::stoi(options["skip"]);
	}
	rjGen.seed(samplerSkip);

	// Adjacency List traversal selector
	std::vector<unsigned long> next{10};
	std::mt19937 nchoiceGen;
	if (options.find("next")!=options.end()) {
		next.clear();
		std::string samples = options["next"];
		{
			std::vector<std::string> splitted = split(samples,',');
			transform(splitted.begin(),splitted.end(),std::back_inserter(next),
			          [](std::string i){ unsigned long j = std::stoul(i); return j; });
		}
		std::sort(next.begin(), next.end());
	}
	//

	// Randomly choosing the jumped vertex
	std::uniform_int_distribution<unsigned long> rv;
	rv = std::uniform_int_distribution<unsigned long>(0, vCount-1);

	// Jump probability
	std::uniform_real_distribution<double> rj{0.0,1.0};

	double jumpProb = 0.4;
	if (options.find("jumpProb")!=options.end()) {
		std::istringstream i(options["jumpProb"]);
		i >> jumpProb;
	}
    std::queue<unsigned long> operandSize;

	for (unsigned long samplerNext : next) {
		nchoiceGen.seed(samplerNext);
		unsigned long counter = 0;

		// first visited vertex, u
		unsigned long u = rv(rvGen);
		boost::dynamic_bitset<> db{vCount};
        db.resize(vCount, false);
        db.set(u, true);

        std::unordered_multimap<unsigned long, unsigned long> adjListtoSerialize;
        std::unordered_set<unsigned long> nVert;

		for (unsigned long size : l) {
            operandSize.push(size);

            if (vCount > 0 ) do {
				int v;
				int step;
                std::unordered_map<unsigned long, std::vector<std::pair<bool,unsigned long>>>::iterator nuf = map.find(u);
				unsigned long Nusize = 0;
				if (nuf != map.end()) {
					Nusize = nuf->second.size();
				}
				//hasToJump
				double doJump = rj(rjGen);
				if ((Nusize>0) && doJump > jumpProb) {
					step = 0;
					std::uniform_int_distribution<> nuNext(0, Nusize-1);
					do {
						unsigned long idgen = nuNext(nchoiceGen);
						if (!nuf->second[idgen].first) {
							adjSCount[u]++;
						}
						nuf->second[idgen].first = true;
						v = nuf->second[idgen].second;
                        if (!db[v]) {
                            counter++;
                            db[v] = true;
                        }

					} while ((adjSCount[u] == Nusize)&&(step++<Nusize));
					if (step <= Nusize) {
						u = v;
                        if (!db[u]) {
                            counter++;
                            db.set(u,true);
                        }

					} else {
						u = rv(rvGen);
                        if (!db[u]) {
                            counter++;
                            db.set(u,true);
                        }
					}
				} else {
					u = rv(rvGen);
                    if (!db[u]) {
                        counter++;
                        db.set(u,true);
                    }

				}
			} while (counter<size);

            bool hasEdge = false;
            adjListtoSerialize.clear();
            nVert.clear();
            for (std::unordered_map<unsigned long, std::vector<std::pair<bool,unsigned long>>>::iterator it=map.begin(); it!=map.end(); ++it) {
                if (db[it->first]) {
                    for (std::pair<bool,unsigned long> cp : it->second) {
                        if (cp.first) {
                            nVert.insert(it->first);
                            nVert.insert(cp.second);
                            hasEdge = true;
                            adjListtoSerialize.emplace(it->first, cp.second);
                        }
                    }
                }
            }

            std::cout << nVert.size() << " vs. " << operandSize.front() << std::endl;

            if (hasEdge && nVert.size() >= operandSize.front()) {
                std::ofstream myfile;
                std::string filename = graph+"_"+std::to_string(operandSize.front())+"_"+std::to_string(samplerNext)+"_"+std::to_string(jumpProb)+"_"+std::to_string(samplerSkip)+"_"+std::to_string(samplerVertex);
                myfile.open (filename);
                nVert.clear();
                for (auto it = adjListtoSerialize.begin(); it != adjListtoSerialize.end(); ++it) {
                    nVert.insert(it->first);
                    nVert.insert(it->second);
                    myfile << it->first << "\t" << it->second << std::endl;
                    if (nVert.size() >= operandSize.front()) {
                        nVert.clear();
                        operandSize.pop();
                        break;
                    }
                }
                myfile.close();
            }


		}

        while (!operandSize.empty()) {
            std::ofstream myfile;
            std::string filename = graph+"_"+std::to_string(operandSize.front())+"_"+std::to_string(samplerNext)+"_"+std::to_string(jumpProb)+"_"+std::to_string(samplerSkip)+"_"+std::to_string(samplerVertex);
            myfile.open (filename);
            nVert.clear();
            for (auto it = adjListtoSerialize.begin(); it != adjListtoSerialize.end(); ++it) {
                nVert.insert(it->first);
                nVert.insert(it->second);
                myfile << it->first << "\t" << it->second << std::endl;
                if (nVert.size() >= operandSize.front()) {
                    nVert.clear();
                    operandSize.pop();
                    break;
                }
            }
            myfile.close();
        }
	}
}
