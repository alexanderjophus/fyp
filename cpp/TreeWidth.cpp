#include <LEDA/graph/graph.h>
#include <LEDA/core/slist.h>
#include "TreeWidth.h"
#include <algorithm>
#include <list>
#include <vector>
#include <iterator>
#include <queue>
#include <set>
#include <utility>
#include <unistd.h>

#define MAXTW 65535
#define MINTW 0

int TreeWidth::Q(std::vector<LEDA::node> S, LEDA::node v) {
  //reachability size of G from v
  int Qc = 0;
  LEDA::node u;
  std::set<LEDA::node> marked;
  std::list<LEDA::node> DFSList;
  marked.insert(v);
  forall_adj_nodes(u,v) {DFSList.push_back(u);}
  while (!DFSList.empty()) {
    LEDA::node curr = DFSList.front();
    DFSList.pop_front();
    if (!(marked.count(curr))) {
      marked.insert(curr);
      if (std::find(S.begin(), S.end(), curr) != S.end()) {
        forall_adj_nodes(u,curr) {DFSList.push_back(u);}
      }
      else {++Qc;}
    }
  }
  return Qc;
}

int TreeWidth::Q(std::set<LEDA::node> S, LEDA::node v) {
  //reachability size of G from v
  int Qc = 0;
  LEDA::node u;
  std::set<LEDA::node> marked;
  std::list<LEDA::node> DFSList;
  marked.insert(v);
  forall_adj_nodes(u,v) {DFSList.push_back(u);}
  while (!DFSList.empty()) {
    LEDA::node curr = DFSList.front();
    DFSList.pop_front();
    if (!(marked.count(curr))) {
      marked.insert(curr);
      if (S.count(curr) > 0) {
        forall_adj_nodes(u,curr) {DFSList.push_back(u);}
      }
      else {++Qc;}
    }
  }
  return Qc;
}

void TreeWidth::setdiff(std::set<LEDA::node>* Sl, std::set<LEDA::node>* Ss, std::set<LEDA::node>* Sd)
{
  std::set<LEDA::node>::iterator it;
  //for item in set1
  for (it = (*Sl).begin(); it != (*Sl).end(); ++it)
  {
    //if not item in set2
    if ((*Ss).count(*it) == 0)
    {
      (*Sd).insert(*it);
    }
  }
}

void TreeWidth::setunion(std::set<LEDA::node>* S1, std::set<LEDA::node>* S2, std::set<LEDA::node>* Su)
{
  std::set<LEDA::node>::iterator it;
  //for item in set1
  for (it = (*S1).begin(); it != (*S1).end(); ++it)
  {
    (*Su).insert(*it);
  }
  for (it = (*S2).begin(); it != (*S2).end(); ++it)
  {
    (*Su).insert(*it);
  }
}

/*
Psuedo-code from 
  On Exact Algorithms for Treewidth
  by Bodlaender, Fomin, Koster, Kratsch, Thilikos
  pg 7 algorithm 1
  
Set TW({}) = -inf
for i in 1 to n do
  for all sets n \subset V with |S| = n do
    Set TW(S) = min_{v \in S} max {TW(S-{v}),|Q(S-{v},v)|}
  od
od
return TW(V)
*/
std::pair<int, std::vector<LEDA::node>> TreeWidth::Dynamic(LEDA::graph* G, std::map<LEDA::node, char>* iomap)
{
  LEDA::node p;
	int n = (*G).number_of_nodes();
	std::vector<LEDA::node> nodes_vect;
	std::vector<LEDA::node> S, T;
	std::map<std::vector<LEDA::node>, int> TW;
	std::map<std::vector<LEDA::node>, std::pair<std::vector<LEDA::node>,
	                                            LEDA::node>> LOM;
	std::vector<LEDA::node> LO;
	int iterationcounter = 0;
//  Linear ordering

  //std::cout << "~";
  forall_nodes(p, (*G)) {
		//std::cout << (*iomap)[p];
		nodes_vect.push_back(p);
	}
  //std::cout << "~\n";

	//Set TW({}) = -inf
	std::vector<LEDA::node> empty;
	TW.insert(std::pair<std::vector<LEDA::node>,int>(empty,MINTW));

	//for i in 1 to n do
	for (int i=1; i<=n; ++i) {
	  std::vector<bool> v(n);
    std::fill(v.begin() + i, v.end(), true);
    
		//  for all sets n \subset V with |S| = n do
	  do {
			S.erase(S.begin(), S.end());
			iterationcounter++; //remove this for final build
			//std::cout << "S = ";  //IMPORTANT PRINT
			//get's all of S
	    for (int k = 0; k < n; ++k) {
	      if (!v[k]) {
					S.push_back(nodes_vect[k]);
	        //std::cout << (*iomap)[nodes_vect[k]] << " ";  //IMPORTANT PRINT
	      }
	    }
			//std::cout << std::endl; //IMPORTANT PRINT
			TW[S] = MAXTW;

			//Set TW(S) = min_{v \in S} max {TW(S-{v}),|Q(S-{v},v)|}
			for (std::vector<LEDA::node>::iterator it = S.begin(); it != S.end(); ++it) {
//				std::cout << (*iomap)[*it] << "\n";
				
				//*TW(S-{v})
				T = S;
				T.erase(std::remove(T.begin(), T.end(), *it), T.end());
				//TW(S-{v}) */

				//*|Q(S-{v},v)|
				int Qc = Q(T, *it);
				//|Q(S-{v},v)| */

//				std::cout << TW[T] << " " << Qc << std::endl;
				int temp = std::max(TW[T],Qc);
//				TW[S] = (temp > TW[S]) ? TW[S] : temp;
        if (temp < TW[S]) {
          TW[S] = temp;
          LOM[S] = std::pair<std::vector<LEDA::node>,LEDA::node>(T,*it);
        }
			}
			//std::cout << "TW[S]: " << TW[S] << std::endl; //IMPORTANT PRINT
	    //std::cout << "\n";
	  } while (std::next_permutation(v.begin(), v.end()));
	}

  std::vector<LEDA::node> LOV = S;
  std::pair<std::vector<LEDA::node>,LEDA::node> temp;
  for (int i = 0; i < (*G).number_of_nodes(); ++i) {
    temp = LOM[LOV];
    LEDA::node tempn = std::get<1>(temp);
    LO.push_back(tempn);
    LOV = std::get<0>(temp);
  }

  std::reverse(LO.begin(), LO.end());

//	std::cout << "Dynamic iterations: \t"<< iterationcounter << std::endl;
	return std::pair<int, std::vector<LEDA::node>>(TW[S], LO);
}

/*
recursive
  On Exact Algorithms for Treewidth
  by Bodlaender, Fomin, Koster, Kratsch, Thilikos
  pg 7 algorithm 1

TWR(Graph G, Vertex Set L, Vertex Set S)
  if |S| = 1 then
    suppose S = {v}
    return Q(L,v)
  end if
  Set Opt = inf
  for all sets S' \subseteq S, |S|=floor(|S|/2) do
    compute v1 = TWR(G,L,S')
    compute v2 = TWR(G,L U S', S - S')
    Set Opt = min {Opt,max{v1,v2}}
  end for
  return Opt
*/

int TreeWidth::Recursive(LEDA::graph* G, std::set<LEDA::node>* L, std::set<LEDA::node>* S, std::map<LEDA::node, char>* iomap, int* iterationcounter)
{
  if ((*S).size() == 1) {
    ++(*iterationcounter);
    return Q( (*L), *(*S).begin() );
  }

  int opt = MAXTW;
  int Spsize = floor((*S).size()/2);
	int n = (*S).size();
	
	std::vector<LEDA::node> nodes_vect((*S).begin(), (*S).end());
  std::vector<bool> v(n);
  std::fill(v.begin() + Spsize, v.end(), true);
	std::set<LEDA::node> SP, LUS, SNSP;
	//nodes_vect vect of all nodes in S

	do {
		SP.clear(); LUS.clear(); SNSP.clear();
//		std::cout << "S: ";
    for (int k = 0; k < n; ++k) {
      if (!v[k]) {
				SP.insert(nodes_vect[k]); //SP = S'
//        std::cout << (*iomap)[nodes_vect[k]] << " ";  //IMPORTANT PRINT
      }
    }
//		std::cout << std::endl;
		//actual computations here
		//compute v1
		int v1 = Recursive(&(*G), &(*L), &SP, &(*iomap), &(*iterationcounter));

		//compute v2
		setunion(&(*L), &SP, &LUS); //LUS = L U S'
		setdiff(&(*S), &SP, &SNSP); //SNSP= S-S'
		int v2 = Recursive(&(*G), &LUS, &SNSP, &(*iomap), &(*iterationcounter));

		//recompute opt
		opt = std::min(opt,std::max(v1,v2));

  } while (std::next_permutation(v.begin(), v.end()));
	return opt;
}
