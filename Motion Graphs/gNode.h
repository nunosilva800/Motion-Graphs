#ifndef _GNODE
#define _GNODE

#include "Edge.h"

using namespace std;

class gNode{
	public:
		gNode();
		~gNode();

		int addEdge(Edge *edge);

		int getNEdges(){return nEdges;}
		//Edge *getEdges();
		Edge *getEdge(int e);

		void setID(int i){ nodeID = i;}
		int getID(){return nodeID;}
		
	private:
		int nodeID;
		int nEdges;
		std::vector<Edge> edges;	
};

#endif