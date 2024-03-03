#ifndef TDALG_H
#define TDALG_H

#include <LEDA/graph/graph.h>
#include <map>
//#include <vector>
#include <set>

class TDAlg
{
private:
  void diff(std::set<LEDA::node>* Xl, std::set<LEDA::node>* Xs, std::set<LEDA::node>* Xd);
  void intersection(std::set<LEDA::node>* Xi, std::set<LEDA::node>* Xj, std::set<LEDA::node>* Xu);
  bool IsAdjacent(LEDA::node* n, LEDA::node* m);
  bool IsIndependent(std::set<LEDA::node>* c, LEDA::node* t);


	std::set<std::set<LEDA::node>> procleaf(LEDA::graph* G, LEDA::graph* T, std::map<LEDA::node, std::set<LEDA::node>>* ItoV, LEDA::node* n);
	std::set<std::set<LEDA::node>> procforg(LEDA::graph* G, LEDA::graph* T, std::map<LEDA::node, std::set<LEDA::node>>* ItoV, std::map<LEDA::node,int>* ItoT, LEDA::node* n);
	std::set<std::set<LEDA::node>> procjoin(LEDA::graph* G, LEDA::graph* T, std::map<LEDA::node, std::set<LEDA::node>>* ItoV, std::map<LEDA::node,int>* ItoT, LEDA::node* n);
	std::set<std::set<LEDA::node>> procintr(LEDA::graph* G, LEDA::graph* T, std::map<LEDA::node, std::set<LEDA::node>>* ItoV, std::map<LEDA::node,int>* ItoT, LEDA::node* n);
	std::set<std::set<LEDA::node>> procnode(LEDA::graph* G, LEDA::graph* T, std::map<LEDA::node, std::set<LEDA::node>>* ItoV, std::map<LEDA::node,int>* ItoT, LEDA::node* n);
public:
  std::set<LEDA::node> MIS(LEDA::graph* G, LEDA::graph* T, std::map<LEDA::node, std::set<LEDA::node>>* ItoV, std::map<LEDA::node,int>* ItoT);
};

#endif
