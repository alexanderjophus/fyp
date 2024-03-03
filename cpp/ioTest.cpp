//*
#include "G6Importer.h"
#include "G6Exporter.h"
#include "TreeWidth.h"
#include "LOtoTD.h"
#include "Naive.h"
#include "TDAlg.h"
#include <LEDA/graph/graph.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>
#include <time.h>
#include <math.h>
#include <utility>

using namespace LEDA;

int main(int argc, char* argv[])
{
  G6Importer importer;  
  G6Exporter exporter;
	TreeWidth TW;
	LOtoTD Decomposition;
	Naive Nv;
	TDAlg TD;
  int DynamicTW;
	std::pair<int, std::vector<LEDA::node>> DynamPair;
  std::pair<LEDA::graph,std::map<LEDA::node,std::set<LEDA::node>>> decomp;
  std::string RawData;
  std::vector<LEDA::node> LOV;

  RawData = argv[1];
    
  LEDA::graph G = importer.decode(RawData);
  
  std::map<LEDA::node, char> iomap;
  int counter = 65;
  LEDA::node nodeio;
  forall_nodes(nodeio, G)
  {
    iomap.insert(std::pair<LEDA::node, char>(nodeio, static_cast<char>(counter)));
    counter++;
  }
  
  leda::list<edge> bleh = G.all_edges();
  
  //start of file io
  std::ofstream output;
  output.open (RawData+".gv");
  output << "graph graphname {" << std::endl;
  
  edge e;
  
  forall_edges(e, G)
    output << iomap[G.source(e)] << " -- " << iomap[G.target(e)] << std::endl;
  node v;
  forall_nodes(v, G) {output << iomap[v] << std::endl;}
//  forall_nodes(v, G) {std::cout << iomap[v] << ": " << v << std::endl;}
  
  std::string label = "label=\"";
  label+=RawData;
  
  output << label+"\";" << std::endl;
  output << "labelloc=top;" << std::endl;
  output << "labeljust=left;" << std::endl;
  
  output << "}" << std::endl;
  output.close();
  //*/
  
  std::cout << "G6 Code: \t\t" << RawData << "\n";
  std::cout << "No nodes: \t\t" << G.number_of_nodes() << "\n";
  std::cout << "No edges: \t\t" << G.number_of_edges() << "\n";  
//  std::cout << "Nodes: \t\t\t" << G.number_of_nodes() << "\n";

  LEDA::graph TEMP = G;
  clock_t beginnn = clock();
  for (int n = 0; n < 1000; ++n) {Nv.NMIS(TEMP);}
  double NNTaken = (double) (clock()-beginnn)/CLOCKS_PER_SEC;
  std::cout << "MIS: \t\t\t" << Nv.NMIS(TEMP) << std::endl;
  std::cout << "Very Naive MIS time: \t" << NNTaken << "\n";

  clock_t beginni = clock();
  for (int n = 0; n < 1000; ++n) {Nv.IMIS(TEMP);}
  double INTaken = (double) (clock()-beginni)/CLOCKS_PER_SEC;
  std::cout << "Improved Naive MIS time:" << INTaken << "\n";
  

	clock_t begind = clock();
	DynamPair = TW.Dynamic(&G, &iomap);

	double DTTaken = (double)(clock()-begind)/CLOCKS_PER_SEC;
	LEDA::node curr;
	//*
	LOV = std::get<1>(DynamPair);
	/*/
	//STUPID WIDTH
	forall_nodes(curr, G) {LOV.push_back(curr);}
	//*/

  DynamicTW = std::get<0>(DynamPair);
	std::cout << "Dynamic Test:\n";
  std::cout << "Dynamic Time: \t\t" << DTTaken << "\n";
  std::cout << "Dynamic Treewidth: \t" << DynamicTW << std::endl;

  /*
	//set L & S
	std::set<LEDA::node> L, S;
	forall_nodes(v, G) {S.insert(v);}
	int iterationcounter=0;
	clock_t beginr = clock();
	int RecursiveTW = TW.Recursive(&G, &L, &S, &iomap, &iterationcounter);
	double RTTaken = (double)(clock()-beginr)/CLOCKS_PER_SEC;
//	std::cout << "Recursive iterations: \t" << iterationcounter << std::endl;
	std::cout << "Recursive Test:\n";
  std::cout << "Recursive Time: \t" << RTTaken << "\n";
  std::cout << "Recursive Treewidth: \t" << RecursiveTW << std::endl;	
  //*/
	
    
  LEDA::graph T;
  std::map<LEDA::node,std::set<LEDA::node>> ItoV;       //maps Xi to vertices
  std::map<LEDA::node, int> ItoT;
  Decomposition.Decompose(&G, &LOV, &iomap, &T, &ItoV, &ItoT);


  //*
  std::set<LEDA::node>::iterator node;
  std::set<LEDA::node> sbag;
  std::set<LEDA::node> tbag;  

  std::ofstream decout;
  decout.open (RawData+"de.gv");
  decout << "digraph decomp {" << std::endl;

  forall_edges(e, T)
  {
    //for all source
    sbag = ItoV[T.source(e)];
    for (node = sbag.begin(); node != sbag.end(); ++node)
    {
      //std::cout << iomap[(*node)];
      decout << iomap[(*node)];
    }
    decout << T.source(e);
    
    //std::cout << " -- ";
    decout << " -> ";
    //for all sink

    tbag = ItoV[T.target(e)];
    for (node = tbag.begin(); node != tbag.end(); ++node)
    {
      //std::cout << iomap[(*node)];
      decout << iomap[(*node)];
    }
    decout << T.target(e);
    //std::cout << std::endl;
    decout << std::endl;
  }
  
  //must also print out all nodes (if complete graph, only 1 bag)
  LEDA::node n;
  forall_nodes(n, T)
  {
    tbag = ItoV[n];
    for (node = tbag.begin(); node != tbag.end(); ++node)
    {
      decout << iomap[(*node)];
    }
    decout << n;
    decout << std::endl;
  }
  
  decout << label+" decomposition\";" << std::endl;
  decout << "labelloc=top;" << std::endl;
  decout << "labeljust=left;" << std::endl;
  
  decout << "}" << std::endl;
  decout.close();
  //*/

//  int MIS = TD.MIS(&G, &T, &ItoV, &ItoT);
  std::set<LEDA::node> MIS;
	clock_t beginntdmis = clock();
  for (int n = 0; n < 1000; ++n) {MIS = TD.MIS(&G, &T, &ItoV, &ItoT);}
	double ntdmisTaken = (double)(clock()-beginntdmis)/CLOCKS_PER_SEC;
  std::cout << "TD MIS: \t\t" << MIS.size() << std::endl;
  for (std::set<LEDA::node>::iterator Mit = MIS.begin(); Mit != MIS.end(); ++Mit) {
    std::cout << iomap[(*Mit)] << " ";
  }
  std::cout << std::endl;
	std::cout << "TD MIS time: \t\t" << ntdmisTaken << std::endl;
  //std::string outputs = exporter.encode(G);
  //std::cout << outputs << "\n";
  
  //*
 	std::fstream OFile;
	OFile.open("Results2.txt", std::fstream::out | std::fstream::app);
    OFile << RawData << "," << G.number_of_nodes() << "," << DynamicTW << "," << NNTaken << "," << INTaken << "," << ntdmisTaken << std::endl;
 	OFile.close();
  //*/

  return 0;
}

//*/
