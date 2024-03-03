#include "Naive.h"
#include <algorithm>    // std::max

bool Naive::isoORpend(LEDA::graph* G) {
	LEDA::node n;
	forall_nodes(n, (*G)) {if ((*G).degree(n) <= 1) {return true;}}
	return false;
}

int Naive::IMIS(LEDA::graph G) {
	if (G.number_of_nodes() == 0) {return 0;}
	else if (isoORpend(&G)) {
		LEDA::node n, nn;
		forall_nodes(n, G) {
			if (G.degree(n) <= 1) {
				forall_adj_nodes(nn,n) {G.del_node(nn);}
				G.del_node(n);
				return (1+IMIS(G));
			}
		}
	}
	else {
		LEDA::graph A,B;
		A = G; B = G;
		LEDA::node na,nb,nn;
		na = A.first_node();
		nb = B.first_node();
		A.del_node(na);
		forall_adj_nodes(nn,nb) {B.del_node(nn);}
		B.del_node(nb);
		return std::max( IMIS(A), (1+IMIS(B)) );
	}
}

int Naive::NMIS(LEDA::graph G) {
	if (G.number_of_nodes() == 0) {return 0;}
	else {
		LEDA::graph A,B;
		A = G; B = G;
		LEDA::node na,nb,nn;
		na = A.first_node();
		nb = B.first_node();
		A.del_node(na);
		forall_adj_nodes(nn,nb) {B.del_node(nn);}
		B.del_node(nb);
		return std::max( NMIS(A), (1+NMIS(B)) );
	}
}
