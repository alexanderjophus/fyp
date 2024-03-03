#ifndef LOTTOTD_H
#define LOTTOTD_H

#include <vector>
#include <map>
#include <LEDA/graph/graph.h>
#include <set>
#include <list>
#include <algorithm>

class LOtoTD
{
private:
  int intersection_size(std::set<LEDA::node> s1, std::set<LEDA::node> s2);
//  std::set<LEDA:node> intersection(std::set<LEDA::node>* Xi, std::set<LEDA::node>* Xj);
  void intersection(std::set<LEDA::node>* Xi, std::set<LEDA::node>* Xj, std::set<LEDA::node>* Xu);
  void diff(std::set<LEDA::node>* Xl, std::set<LEDA::node>* Xs, std::set<LEDA::node>* Xd);
  bool isAdj(LEDA::node* i, LEDA::node* j);

public:
//  std::pair<LEDA::graph, std::map<LEDA::node, std::set<LEDA::node>>> Decompose(LEDA::graph* G, std::vector<LEDA::node>* LO, std::map<LEDA::node, char>* iomap);
  void Decompose(LEDA::graph* G, std::vector<LEDA::node>* LO, std::map<LEDA::node, char>* iomap, LEDA::graph* T, std::map<LEDA::node,std::set<LEDA::node>>* ItoV, std::map<LEDA::node,int>* ItoT);
};

#endif
