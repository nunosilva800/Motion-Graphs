#ifndef _GNODE
#define _GNODE

#include "Edge.h"
#include <vector>

class gNode{
	public:
		gNode();
		~gNode();

		int addEdge(Edge *edge);

		int getNEdges(){return nEdges};
		Edge **getEdges();

	private:
		int nEdges;
		lEdge edges;
		
};

#endif