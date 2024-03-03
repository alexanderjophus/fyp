#include "TDAlg.h"
#include <list>
#include <algorithm>
//join
//introduce
//forget
//leaf

void TDAlg::diff(std::set<LEDA::node>* Xl, std::set<LEDA::node>* Xs, std::set<LEDA::node>* Xd)
{
  std::set<LEDA::node>::iterator it;
  (*Xd).clear();
  //for item in set1
  for (it = (*Xl).begin(); it != (*Xl).end(); ++it)
  {
    //if not item in set2
    if ((*Xs).count(*it) == 0)
    {
      (*Xd).insert(*it);
    }
  }
}

void TDAlg::intersection(std::set<LEDA::node>* Xi, std::set<LEDA::node>* Xj, std::set<LEDA::node>* Xu)
{
  std::set<LEDA::node>::iterator it;
  //for item in set1
  (*Xu).clear();
  for (it = (*Xi).begin(); it != (*Xi).end(); ++it)
  {
    //if item in set2
      if ((*Xj).count(*it))
    {
      (*Xu).insert(*it);
    }
  }
}

bool TDAlg::IsAdjacent(LEDA::node* n, LEDA::node* m) {
  LEDA::node adj;
  forall_adj_nodes(adj, (*n)) {if (adj == (*m)) {return true;}} return false;
}

bool TDAlg::IsIndependent(std::set<LEDA::node>* c, LEDA::node* t) {
  std::set<LEDA::node>::iterator it;
  LEDA::node temp;
  for (it = (*c).begin(); it != (*c).end(); ++it)
  {
    temp = *it;
    if (IsAdjacent(&temp,t)) {return false;}
  }
  return true;
}

std::set<std::set<LEDA::node>> TDAlg::procnode(LEDA::graph* G, LEDA::graph* T, std::map<LEDA::node, std::set<LEDA::node>>* ItoV, std::map<LEDA::node,int>* ItoT, LEDA::node* n) {
	switch ((*ItoT)[(*n)]) {
		case 0: //Join
		  return procjoin(G, T, ItoV, ItoT, n);
		case 1: //Introduce
		  return procintr(G, T, ItoV, ItoT, n);
		case 2: //Forget
		  return procforg(G, T, ItoV, ItoT, n);
		case 3: //Leaf
		  return procleaf(G, T, ItoV, n);
	}
}

std::set<std::set<LEDA::node>> TDAlg::procleaf(LEDA::graph* G, LEDA::graph* T, std::map<LEDA::node, std::set<LEDA::node>>* ItoV, LEDA::node* n) {
  std::set<std::set<LEDA::node>> S;
  std::set<LEDA::node> empty, s, temp;
//  std::set<std::set<LEDA::node> >::iterator Sit;
//  std::set<LEDA::node>::iterator sit;

  s = (*ItoV)[(*n)];
  S.insert(empty);
  S.insert(s);

  return S;
}

std::set<std::set<LEDA::node>> TDAlg::procjoin(LEDA::graph* G, LEDA::graph* T, std::map<LEDA::node, std::set<LEDA::node>>* ItoV, std::map<LEDA::node,int>* ItoT, LEDA::node* n) {
  LEDA::node childl = (*T).target((*T).first_out_edge(*n));
  LEDA::node childr = (*T).target((*T).last_out_edge(*n));
  LEDA::node* clp = &childl;
  LEDA::node* crp = &childr;
  std::set<std::set<LEDA::node>>::iterator lit, rit;
  std::set<LEDA::node>::iterator it;
  std::set<LEDA::node> l, r, u;
  std::set<LEDA::node> templ, tempr;
  std::set<std::set<LEDA::node>> S, L, R;
  
  L = procnode(G, T, ItoV, ItoT, clp);
  R = procnode(G, T, ItoV, ItoT, crp);
  
  for (lit = L.begin(); lit != L.end(); ++lit) {
    templ = *lit;
    l.clear();
    intersection(&templ, &(*ItoV)[(*n)], &l);
    for (rit = R.begin(); rit != R.end(); ++rit) {
      tempr = *rit;
      r.clear();
      intersection(&tempr, &(*ItoV)[(*n)], &r);
      if (l == r) {
        for(it = templ.begin();  it != templ.end(); ++it) {tempr.insert(*it);}
        S.insert(tempr);
      }
    }  
  }
  return S;
}

std::set<std::set<LEDA::node>> TDAlg::procintr(LEDA::graph* G, LEDA::graph* T, std::map<LEDA::node, std::set<LEDA::node>>* ItoV, std::map<LEDA::node,int>* ItoT, LEDA::node* n) {
  LEDA::node child = (*T).target((*T).first_out_edge(*n));
  LEDA::node* cp = &child;
  std::set<std::set<LEDA::node>> S, C;
  std::set<std::set<LEDA::node>>::iterator it;
  std::set<LEDA::node> parset, chiset, tempset;
  std::set<LEDA::node> c, temp;
  std::set<LEDA::node>::iterator cit;
  parset = (*ItoV)[(*n)]; chiset = (*ItoV)[child];
  std::set<LEDA::node>::iterator Iit;

  C = procnode(G, T, ItoV, ItoT, cp);
  S = C;
  diff(&parset, &chiset, &tempset);
  LEDA::node t = (*tempset.begin());
  for (it = C.begin(); it != C.end(); ++it) {
    temp = *it;
    if (IsIndependent(&temp, &t)) {
      temp.insert(t);
      S.insert(temp);
    }
  }
  return S;
}

std::set<std::set<LEDA::node>> TDAlg::procforg(LEDA::graph* G, LEDA::graph* T, std::map<LEDA::node, std::set<LEDA::node>>* ItoV, std::map<LEDA::node,int>* ItoT, LEDA::node* n) {

  LEDA::node child = (*T).target((*T).first_out_edge(*n));
  LEDA::node* cp = &child;
  std::set<LEDA::node> curr = (*ItoV)[(*n)];
  std::set<std::set<LEDA::node>> S, C;
  std::set<LEDA::node> c, temp1, temp2, In1, In2, max;
  std::set<std::set<LEDA::node>>::iterator it1, it2;
  bool added;

  C = procnode(G, T, ItoV, ItoT, cp);
  for (it1 = C.begin(); it1 != C.end(); ++it1) {
    max.clear();
    temp1 = (*it1);
//    max = temp1;
    //max should be empty set?
    In1.clear();
    intersection(&curr, &temp1, &In1);
    for (it2 = C.begin(); it2 != C.end(); ++it2) {
//      if ((*it1) == (*it2)) {continue;}
      temp2 = (*it2);
      In2.clear();
      intersection(&curr, &temp2, &In2);
      if (In1 == In2) {
        max = (max.size() > temp2.size()) ? max : temp2;
      }
    }
    S.insert(max);
  }
  return S;
}

std::set<LEDA::node> TDAlg::MIS(LEDA::graph* G, LEDA::graph* T, std::map<LEDA::node, std::set<LEDA::node>>* ItoV, std::map<LEDA::node,int>* ItoT) {
  //get a root node
  LEDA::node r = (*T).last_node();
  while ((*T).in_edges(r).size() != 0) {r = (*T).source((*T).first_in_edge(r));}
  LEDA::node* rp = &r;

  //call procnode with node
  std::set<std::set<LEDA::node>> S;
  std::set<LEDA::node> s;
  int size, max;
  std::set<LEDA::node> MIS;

  max = 0;
  S = procnode(G, T, ItoV, ItoT, rp);
  for (std::set<std::set<LEDA::node>>::iterator Sit = S.begin(); Sit != S.end(); ++Sit) {
    size = (*Sit).size();
    if (size > max) {
      max = size;
      MIS = (*Sit);
    }
  } 
  return MIS;
}
