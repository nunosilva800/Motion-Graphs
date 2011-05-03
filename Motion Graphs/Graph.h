#ifndef _GRAPH
#define _GRAPH

#include "Ninja.h"
#include "gNode.h"
#include "dMap.h"


class Graph{
	public:
		Graph();
		~Graph();

		void constructGraph(Ninja **ninjas, int nNinjas);

		void addNode(gNode node);
		void addEdge(gNode);
		gNode* existNode(gNode *node);

		int getNNodes(){return this->nNodes;}
		gNode* getNode(int node);

	private:
		int nNodes;
		std::vector<gNode> nodes;
};


#endif