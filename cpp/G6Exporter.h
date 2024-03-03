#ifndef G6EXPORTER_H_
#define G6EXPORTER_H_

#include "GraphExporter.h"
#include <string>

class G6Exporter: public SuperExporter
{
public:
	  char totalNodes(LEDA::graph G);
	  std::string encode(LEDA::graph G);

};

#endif /* G6EXPORTER_H_ */
