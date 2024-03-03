#include <LEDA/graph/graph.h>
#include <string>
#include "GraphExporter.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include "matrix.h"

class G6Exporter: public SuperExporter
{
public:
  int totalNodes(LEDA::graph G)
  {
    //count the number of edges
    int intNodes = G.number_of_nodes();
    return intNodes;
  }

  std::string encodeR(LEDA::graph G, int NNo, char CNo)
  {
	  //adds number of nodes to string... 4 is now C etc
	  std::string encodedOutput;
	  std::stringstream ss;
	  ss << CNo;
	  ss >> encodedOutput;

	  //makes a map of nodes, giving each a usable number for storage in the matrix
	  std::unordered_map<LEDA::node,short> M;
	  LEDA::node n;
	  int i = 0;
	  forall_nodes(n, G)
	  {
	    M.insert(std::pair<LEDA::node,short>(n,i));
	    i++;
	  }

	  //given a 0 matrix writes 1's for all edges in the graph
	  Matrix R(NNo,NNo);
	  LEDA::edge e;
	  forall_edges(e, G)
	  {
	    //place end points of n as 1's on matrix
	    LEDA::node u = G.source(e);
	    LEDA::node v = G.target(e);
	    std::unordered_map<LEDA::node,short>::const_iterator gotu = M.find(u);
	    std::unordered_map<LEDA::node,short>::const_iterator gotv = M.find(v);
	    //std::cout << " " << gotu->second << "  " << gotv->second << std::endl;
	    R.setItem(gotv->second,gotu->second,1);
	  }



	  //
	  int counter = 0;
	  short bit = 1;
    int encodedGraphByte=0;
    for (int j = 1; j < (NNo); j++) {
      for (int i = 0; i < j; i++) {
    	//make array size 6 to store bits
        if (R.getItem(i,j)) {
          encodedGraphByte = encodedGraphByte | 1<<(5-counter);
        }
        //std::cout << encodedGraphByte << "\n";
        counter++;
        if (counter == 6) {
          //convert 6 bits into ascii and append to encodedOutput
          encodedGraphByte += 63;
          counter = 0;
          char encodedChar = (char) encodedGraphByte;
      	  encodedOutput += encodedChar;
          //std::cout << encodedGraphByte << "\n";
          encodedGraphByte = 0;
        }
      }
    }
    //if array is not empty then pad, convert to ascii and append to encoded output
    if (counter != 0){
      //convert remaining bits into ascii and append to encodedOutput
      encodedGraphByte += 63;
      char encodedChar = (char) encodedGraphByte;
      encodedOutput += encodedChar;
      //std::cout << encodedGraphByte << "\n";
    }


	  return encodedOutput;
  }

  std::string encode(LEDA::graph G);
};

std::string G6Exporter::encode(LEDA::graph G)
{
  int NNo = totalNodes(G);
  //encodeR
  char CNo = (char) 63 + NNo;
  //N is now added to the string encodedN, NNo holds number of nodes

  std::string encodedOutput = encodeR(G, NNo, CNo);

  //std::string encodedOutput = "CT";

  return encodedOutput;
}
