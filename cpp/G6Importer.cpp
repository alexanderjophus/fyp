#include <iostream>
#include <LEDA/graph/graph.h>
#include <string>
#include <cmath>
#include <vector>
#include "GraphImporter.h"
#include "matrix.h"

std::vector<bool> DecToBin(int);

//G6Importer, this class has the functions to convert G6 into what we want
class G6Importer: public SuperImporter
{
public:
  signed int DecodeN(std::string G6Raw)
  {
    int ReturnValue = 0;
    
    //if first bit is < 126
    if ((int) G6Raw.at(0) < 126) return ((signed int) G6Raw.at(0)-63);

    //if second bit is < 63 (after subtraction)
    else if ((int) G6Raw.at(1) < 126)
    {
      //b1*2^12+(b2*2^6)+b3
      int ReturnValue = 0;
      for (int i = 2; 0<=i; --i)
      {
        /*
        LOOK INTO BIT SHIFTING, more efficient
        */
        //std::cout << i << "\n";
        ReturnValue += ((G6Raw.at(i+1)-63) * pow(2, (2-i)*6));
        std::cout << G6Raw.at(i+1) << " " << ReturnValue << "\n";
      }
      //return bytes 1,2,3
    }
    
    //else
    else
    {
      int ReturnValue = 0;
      for (int i = 6; 1<=i; --i)
      {
        ReturnValue += ((G6Raw.at(i+1)-63) * pow(2, (6-i)*6));
        std::cout << G6Raw.at(i+1) << " " << ReturnValue << "\n";
      }
      //return bytes 1,2,3,4,5,6,7
    }
    
    return ReturnValue;
  }
  
  Matrix DecodeR(std::string G6Raw, int TotalNodes)
  {
    //convert the sequence of numbers given the total nodes into a binary vector
		int ByteStart = 0;
    if (TotalNodes < 63){
			ByteStart = 1;
    }
		else {
			ByteStart = 2;
		}
		
		std::vector<bool> FullVector; 
    for (unsigned i=ByteStart; i<G6Raw.length(); ++i)
    {
      //convert this to binary
			std::vector<bool> ByteVector = DecToBin(G6Raw.at(i)-63);
			
			FullVector.insert(FullVector.end(), ByteVector.begin(), ByteVector.end());
    }
    

    //from the vector place them into the matrix
    Matrix R(TotalNodes, TotalNodes);
    
		for (int i = 0; i < TotalNodes; i++) R.setItem(i,i,0);


    int k=0;
    for (int i = 0; i < (TotalNodes); i++)
    {
      for (int j = 0; j < i; j++)
      {
        //std::cout << "k is " << FullVector[k] << "\n";
        if (FullVector[k] == 1) R.setItem(i,j,true);
				else R.setItem(j,i,false);
        k++;
      }
    }

    return R;
  }
  
  LEDA::graph decode(std::string);
};


LEDA::graph G6Importer::decode(std::string G6Raw)
{
  //decode N
  signed int TotalNodes = DecodeN(G6Raw);
  //std::cout << TotalNodes << "\n";

  //decode R
  Matrix R = DecodeR(G6Raw, TotalNodes);

  LEDA::graph G;
  //from data in R, make the graph in G
  G.make_undirected();


  /*
   *
   * MEND THIS!!!!!!!!!!!!!!!!!1
   */
		LEDA::node A[TotalNodes];
		
		for (int i = 0; i < TotalNodes; i++) A[i] = G.new_node();

		for (int i = 0; i < TotalNodes; i++) {
			for (int j = 0; j < i; j++) {
				if (R.getItem(i,j)) G.new_edge(A[i],A[j]);
			}
		}
		
		//G.print();

  return G;
}

std::vector<bool> DecToBin(int dec)
{
	std::vector<bool> buffer(6);
	for (int i = 0; i < 6; i++) {
		buffer[5-i] = dec & (1<<i);
	}
	
	return buffer;
}
