#include "LOtoTD.h"

//std::pair<LEDA::graph, std::map<LEDA::node,std::set<LEDA::node>>> LOtoTD::Decompose(LEDA::graph* G, std::vector<LEDA::node>* LO, std::map<LEDA::node, char>* iomap) {
int LOtoTD::intersection_size(std::set<LEDA::node> s1, std::set<LEDA::node> s2)
{
  std::set<LEDA::node>::iterator it;
  int count = 0;
  //for item in set1
  for (it = s1.begin(); it != s1.end(); ++it)
  {
    //if item in set2
    if (s2.count(*it))
    {
      count++;
    }
  }
  return count;
}

void LOtoTD::intersection(std::set<LEDA::node>* Xi, std::set<LEDA::node>* Xj, std::set<LEDA::node>* Xu)
{
  std::set<LEDA::node>::iterator it;
  //for item in set1
  for (it = (*Xi).begin(); it != (*Xi).end(); ++it)
  {
    //if item in set2
      if ((*Xj).count(*it))
    {
      (*Xu).insert(*it);
    }
  }
}

/*
void LOtoTD::setunion(std::set<LEDA::node>* Xi, std::set<LEDA::node>* Xj, std::set<LEDA::node>* Xu)
{
  std::set<LEDA::node>::iterator it;
  //for item in set1
  for (it = (*Xi).begin(); it != (*Xi).end(); ++it)
  {
    //if item in set2
      if ((*Xj).count(*it))
    {
      (*Xu).insert(*it);
    }
  }
}
//*/

void LOtoTD::diff(std::set<LEDA::node>* Xl, std::set<LEDA::node>* Xs, std::set<LEDA::node>* Xd)
{
  std::set<LEDA::node>::iterator it;
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

bool LOtoTD::isAdj(LEDA::node* i, LEDA::node* j)
{
	LEDA::node temp;
	forall_adj_nodes(temp, (*i))
	{
		if (temp == (*j)) {return true;}
	}
	return false;
}

void LOtoTD::Decompose(LEDA::graph* G, std::vector<LEDA::node>* LO, std::map<LEDA::node, char>* iomap, LEDA::graph* T, std::map<LEDA::node,std::set<LEDA::node>>* ItoV, std::map<LEDA::node,int>* ItoT) {
	std::set<std::set<LEDA::node>> decomp;
  std::set<LEDA::node> bag;
  std::set<LEDA::node> bagged;
  LEDA::node currnode, prevnode, largest;
  int biggestint, currint;
  std::set<LEDA::node> intree, notintree;
	std::vector<LEDA::edge> chrdl;
	LEDA::edge chrde;

	//prep for algorithm
//	(*T).make_undirected();
	//BAGGING FROM MODIFIED LO

  //LEFT TO RIGHT - COMPLETE THE NEIGHBOURHOOD OF THE NODES TO THE RIGHT
  std::set<LEDA::node> prev;
  for (std::vector<LEDA::node>::iterator it = (*LO).begin(); it != (*LO).end(); ++it)
  {
    //std::cout << "it; " << (*iomap)[*it] << std::endl;
    prev.insert(*it);
    LEDA::node i, j;
    //for all neighbours i to the right
    forall_adj_nodes(i, *it)
    {
      //std::cout << "i; " << (*iomap)[i] << std::endl;
      
      //for all neighbours j to the right
      forall_adj_nodes(j, *it)
      {
        //std::cout << "j; " << (*iomap)[j] << std::endl;
        if (!( (i == j) || (prev.count(i)) || (prev.count(j)) || (isAdj(&i,&j)) ))
        {
          //maybe check if {i,j} is already an edge
          chrde = (*G).new_edge(i,j);
					chrdl.push_back(chrde);
        }
      }
    }
  }

  //right to left
  for (std::vector<LEDA::node>::iterator it = (*LO).end()-1 ; it != (*LO).begin()-1; --it)
  {
		LEDA::list<LEDA::node> adjlist = (*G).adj_nodes(*it);
		adjlist.push_back(*it);
    bagged.insert(*it);
		
		//std::cout << (*iomap)[(*it)] << "\n"; IMPORTANT PRINT
    while (!adjlist.empty()) {
			LEDA::node curr = adjlist.front();
			adjlist.pop_front();
      if ((bagged.count(curr))) {
				bag.insert(curr);
				//std::cout << (*iomap)[curr];      IMPORTANT PRINT
			}
    }
		//std::cout << "\n";                    IMPORTANT PRINT

//*
//		CODE HERE TO REMOVE SETS THAT ARE SUBSETS OF OTHER SETS
	//for all subsets currently in map
		for (std::map<LEDA::node,std::set<LEDA::node>>::iterator mapit=(*ItoV).begin(); mapit != (*ItoV).end(); ++mapit) {
		//if it's a subset of the current bag
		  if (std::includes(bag.begin(), bag.end(), mapit->second.begin(), mapit->second.end())) {
			//remove it's set
        
			//remove the node from the tree
			  (*T).del_node(mapit->first);
			}
		}
//*/
    
    currnode = (*T).new_node();
//    std::cout << currnode << std::endl;
    (*ItoV)[currnode] = bag;
    bag.clear();
  }
//  std::cout << "\n";

	//make G original (non chordal again)
	std::vector<LEDA::edge>::iterator eit;
	for (eit = chrdl.begin(); eit != chrdl.end(); ++eit) {(*G).del_edge(*eit);}

  std::vector<LEDA::node> I, J;
  LEDA::node v, besti, bestj;
  std::vector<LEDA::node>::iterator iit, jit;
  int icounter, iindex;
  int intersize, currinter;

//arbitrarily add bag to tree
//pick & remove first I to J
  forall_nodes(v, (*T)){I.push_back(v);}

  J.push_back(I.back());
  I.pop_back();

//while tree not full
//while J.size != (*T).no nodes
  while (!(I.empty()))
  {
    intersize = 0;

//  for each bag in tree
//  for J --
    for (jit = J.begin(); jit != J.end(); ++jit)
    {
      icounter = 0;

//    for each bag not in tree
//    for I
      for (iit = I.begin(); iit != I.end(); ++iit)
      {
      
//    check intersection_size
//    intersection_size (ItoV(I),ItoV(J)
        currinter = intersection_size((*ItoV)[*iit],(*ItoV)[*jit]);
        if (currinter > intersize)
        {
          besti = *iit;
          bestj = *jit;
          intersize = currinter;
          iindex = icounter;
        }
        ++icounter;
      }
    }
//  add bag with edge connecting largest intersection_size
    (*T).new_edge (bestj, besti);

    J.push_back(I[iindex]);
//    std::cout << iindex << std::endl;
    I.erase(I.begin()+iindex);

  }
  
  
  /*
  TREE DECOMPOSITION TO NICE TREE DECOMPOSITION
  */

  //PICK ROOT ARBITRARILY (leaf for simplicity)
  LEDA::node r;
  LEDA::node* rp;
//  forall_nodes(b, (*T)) {if ((*T).degree(b) == 1) {rp = &b; break;}}

  r =(*T).last_node();
  while ((*T).in_edges(r).size() != 0) {r = (*T).source((*T).first_in_edge(r));}
  
  std::set<LEDA::node>::iterator node;
  std::set<LEDA::node> marked;
  std::set<LEDA::node> abag, XuSet, Xdiff;
  std::list<LEDA::node> BFSList;
  std::list<LEDA::edge> elist1, elist2;
  std::list<LEDA::edge>::iterator it, lit, rit;
  LEDA::list<LEDA::edge> temp;
  LEDA::node u; LEDA::edge e;
  LEDA::node Xl, Xr, Xu;
  LEDA::edge f;
  marked.insert(r);
//  forall_out_edges(e,r) {BFSList.push_back((*T).target(e));}
  BFSList.push_back(r);
  while (!BFSList.empty()) {
  //make depth first search through *T from r
    LEDA::node curr = BFSList.front();
    BFSList.pop_front();
    /*
    abag = (*ItoV)[curr];
    std::cout << "!: ";
    for (node = abag.begin(); node != abag.end(); ++node) {
      std::cout << (*iomap)[(*node)];
    }
    std::cout << std::endl;
    //*/
    
    //if more than 1 child
    if ((*T).out_edges(curr).size() > 1) {
      (*ItoT)[curr] = 0;
      elist1.clear();
      elist2.clear();
      temp = (*T).out_edges(curr);
      forall(f,temp) {elist1.push_back(f);}
      
      //elist2 = elist1.splice(midway)
      //it point midway
      it = elist1.begin();
      std::advance(it, (elist1.size()/2));
      elist2.splice(elist2.begin(), elist1, it, elist1.end());

      Xl = (*T).new_node();
      Xr = (*T).new_node();
      (*T).new_edge(curr, Xl);
      (*T).new_edge(curr, Xr);

      (*ItoV)[Xl] = (*ItoV)[curr];
      (*ItoV)[Xr] = (*ItoV)[curr];
      
      for(lit = elist1.begin(); lit != elist1.end(); ++lit) {
        //add edge lit Xl
        (*T).new_edge(Xl, (*T).target(*lit));
        //remove edge lit curr
        (*T).del_edge(*lit);
      }
      
      for(rit = elist2.begin(); rit != elist2.end(); ++rit) {
        //add edge lit Xl
        (*T).new_edge(Xr, (*T).target(*rit));
        //remove edge lit curr
        (*T).del_edge(*rit);
      }
      BFSList.push_back(Xl);
      BFSList.push_back(Xr);
    }
    
    //else only 1 child
    //*
    else if ((*T).out_edges(curr).size() == 1) {
      //bridge gap between parent and child
      std::set<LEDA::node> p, c;
      p = (*ItoV)[curr];
      LEDA::edge ce = *(*T).out_edges(curr).begin();
      LEDA::node cn = (*T).target(ce);
      c = (*ItoV)[cn];
      (*ItoT)[curr] = 1;
      
      Xu = (*T).new_node();
      XuSet.clear();
      intersection(&p, &c, &XuSet);
      (*ItoV)[Xu] = XuSet;
      (*ItoT)[Xu] = 2;
      LEDA::edge Eu = (*T).new_edge(curr,Xu);
      LEDA::edge Ed = (*T).new_edge(Xu,cn);
      (*T).del_edge(ce);
      BFSList.push_back(cn);
      
      //work up from Xu to curr
      //*
      Xdiff.clear();
      diff(&p, &XuSet, &Xdiff);
      LEDA::node Xnew = Xu;
      std::set<LEDA::node> XuCopy = XuSet;
      while (Xdiff.size() > 1) {
        //std::cout << (*iomap)[*Xdiff.begin()];
        std::set<LEDA::node>::iterator tempnode = Xdiff.begin();
        Xdiff.erase(Xdiff.begin());
        XuCopy.insert(*tempnode);
        LEDA::node Xold = Xnew;
        Xnew = (*T).new_node();
        (*ItoV)[Xnew] = XuCopy;
        (*ItoT)[Xnew] = 1;
        LEDA::edge Etemp = Eu;
        Eu = (*T).new_edge(curr, Xnew);
        LEDA::edge Eb = (*T).new_edge(Xnew, Xold); 
        (*T).del_edge(Etemp);
      }
      //*/
      //std::cout << std::endl;

      //work down from Xu to cn
      //*
      Xdiff.clear();
      diff(&c, &XuSet, &Xdiff);
      Xnew = Xu;
      XuCopy = XuSet;
      while (Xdiff.size() > 1) {
        //std::cout << (*iomap)[*Xdiff.begin()];
        std::set<LEDA::node>::iterator tempnode = Xdiff.begin();
        Xdiff.erase(Xdiff.begin());
        XuCopy.insert(*tempnode);
        LEDA::node Xold = Xnew;
        Xnew = (*T).new_node();
        (*ItoV)[Xnew] = XuCopy;
        (*ItoT)[Xnew] = 2;
        LEDA::edge Etemp = Ed;
        LEDA::edge Eb = (*T).new_edge(Xold, Xnew); 
        Ed = (*T).new_edge(Xnew, cn);
        (*T).del_edge(Etemp);
      }
      //*/
    }
    else { //else (*T).out_edges(curr).size() == 0
      //whittle down to bag size of 1
      std::set<LEDA::node> leftovers = (*ItoV)[curr];
      (*ItoT)[curr] = 1;
      LEDA::node old = curr;
      while (leftovers.size() > 1) {
        //reduce leftovers
        std::set<LEDA::node>::iterator tempnode = leftovers.begin();
        leftovers.erase(leftovers.begin());
        //create new node
        LEDA::node newnode = (*T).new_node();
        //contains bag = leftovers
        (*ItoV)[newnode] = leftovers;
        if (leftovers.size() > 1) {(*ItoT)[newnode] = 1;}
        else {(*ItoT)[newnode] = 3;}
        //create edge
        (*T).new_edge(old, newnode);
        old = newnode;
      }
    }
    //std::cout << "\n";
  }
  /*
  r =(*T).last_node();
  while ((*T).in_edges(r).size() != 0) {r = (*T).source((*T).first_in_edge(r));}
  
  std::set<LEDA::node> newbag;
  LEDA::node newnode;
  while ((*ItoV)[r].size() > 1) {
    newnode = (*T).new_node();
    (*T).new_edge(newnode,r);
    //    (*ItoV)[currnode] = bag;
    newbag = (*ItoV)[r];
    newbag.erase(newbag.begin());
    (*ItoV)[newnode] = newbag;
    (*ItoT)[newnode] = 2;
    r = newnode;
  }  
  //*/
}
