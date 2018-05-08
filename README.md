# graphSampler
> Giacomo Bergami, &copy; 2017-2018

Samples random graphs generated from an adjacency list, where source and target vertices are tab separated.
This graph sampler uses a random graph visit, after which different subgraphs are visited from the initial subgraph.

The program provides the subgraph as an adjacency list in multiple subgraphs located in the same path where the main adjacency list is stored.

This project was used to sample graphs for the following personal papers:
* G. Bergami, M. Magnani, D. Montesi. “[A Join Operator for Property Graphs](http://ceur-ws.org/Vol-1810/GraphQ_paper_04.pdf)”. Proceedings of Sixth International Workshop on Querying Graph Structured Data, Venice, Italy, March 2017 (GraphQ 2017)
* G. Bergami, M. Magnani, D. Montesi. 2016. “[On Joining Graphs](https://arxiv.org/abs/1608.05594)”. CoRR abs/1608.05594 (2016).

## Configuration file
 * **graph**: path for the adjacency list file (default: *graph.txt*).
 * **samples**: comma separated subgraph size (default: *10*).
 * **vertex**: seed for randomly selecting a vertex. By chanhing this value we select a different vertex when starting the graph visit (default: *0*).
 * **skip**: seed for randomly interrupting the graph visit and continuing in another part of the graph. This is suitable when the graph is visited or we stuck in an ending node (default: *1*).
 * **jumpProb**: probability of performing a random jump and interrupting the graph traversal process (default: *0.4*).
 * **next**: seed for selecting different adjacency vertices given the same initial visited vertex (default: *10*).
 
