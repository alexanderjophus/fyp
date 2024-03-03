#ifndef G6EXPORTER_H
#define G6EXPORTER_H

#include <string>
#include "matrix.h"
#include <LEDA/graph/graph.h>
#include "GraphImporter.h"

class G6Importer: public SuperImporter
{
  public:
  signed int DecodeN(std::string G6Raw);
  Matrix DecodeR(std::string G6Raw, int TotalNodes);
  LEDA::graph decode(std::string G6Raw);
};

#endif
