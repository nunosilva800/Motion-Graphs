#ifndef _GRAPH
#define _GRAPH

#include "Ninja.h"
#include "gNode.h"


class Graph{
	public:
		Graph();
		~Graph();

		void constructGraph(Ninja *ninjas, int nNinjas);

		void addNode(gNode node);
		gNode* existNode(gNode node);

		int getNNodes(){return this->nNodes;}
		gNode getNode(int i){return this->nodes[i];};

	private:
		int nNodes;
		std::vector<gNode> nodes;
};


#endif