#ifndef _GRAPH
#define _GRAPH

#include "Ninja.h"
#include "gNode.h"

class Graph{
	public:
		Graph();
		~Graph();

		void constructGraph(Ninja *ninjas, int nNinjas);

		bool addNode(gNode *node);
		gNode* existNode(gNode *node);


	private:
		int nNodes;
		gNode *nodes;



		
};


#endif