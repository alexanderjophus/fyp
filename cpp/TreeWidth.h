#ifndef TREEWIDTH_H
#define TREEWIDTH_H

#include <LEDA/graph/graph.h>
#include <map>
#include <vector>
#include <set>

class TreeWidth 
{
private:
	bool path(LEDA::graph G, std::vector<LEDA::node> S, LEDA::node w, LEDA::node v);
	int Q(std::vector<LEDA::node> T, LEDA::node v);
	int Q(std::set<LEDA::node> T, LEDA::node v);
	void setunion(std::set<LEDA::node>* S1, std::set<LEDA::node>* S2, std::set<LEDA::node>* Su);
	void setdiff(std::set<LEDA::node>* Sl, std::set<LEDA::node>* Ss, std::set<LEDA::node>* Sd);


public:
	std::pair<int, std::vector<LEDA::node>> Dynamic(LEDA::graph* G, std::map<leda::node, char>* iomap);
	int Recursive(LEDA::graph* G, std::set<LEDA::node>* L, std::set<LEDA::node>* S, std::map<LEDA::node, char>* iomap, int* iterationcounter);
};

#endif
