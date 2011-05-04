#ifndef _GRAPH
#define _GRAPH

#include "Ninja.h"
#include "gNode.h"
#include "dMap.h"


class Graph{
	public:
		Graph();
		~Graph();

		void constructGraph(Ninja **ninjas, int nNinjas, float threshold, int nCoincidents);

		int addNode(gNode *node);
		//void addEdge(gNode);
		gNode* existNode(gNode *node);

		int getNNodes(){return this->nNodes;}
		gNode* getNode(int node);

	private:
		int nNodes;
		std::vector<gNode> nodes;

		void createTransition(Motion *m1, int mPos1, std::vector<int> tPos1, 
							  Motion *m2, int mPos2, std::vector<int> tPos2, 
							  dMap *map, int r);

		int **indexes;
		
		void initIndexes(Ninja **ninjas, int nNinjas);
};


#endif