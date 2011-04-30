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

class dMap{
	public:
		float **compareMotions(Motion *m1, Motion m2){

		}

	private:
		float ***differenceMap;
		Motion *relations[2];
}
#endif