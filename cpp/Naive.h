#ifndef NAIVE_H
#define NAIVE_H

#include <LEDA/graph/graph.h>
#include <map>
#include <vector>

class Naive
{
private:
	bool isoORpend(LEDA::graph* G);

public:
	int IMIS(LEDA::graph G);
	int NMIS(LEDA::graph G);
};
#endif
