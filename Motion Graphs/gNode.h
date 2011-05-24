#ifndef _GNODE
#define _GNODE
#include "stdafx.h"
#include "Edge.h"

using namespace std;

class gNode{
	public:
		gNode();
		gNode(int id);
		~gNode();

		int addEdge(Edge *edge);

		int getNEdges(){return nEdges;}
		//Edge *getEdges();
		Edge *getEdge(int e);

		void setID(int id){nodeID = id;}
		int getID(){return nodeID;}
		bool removeEdge(int i);
		
	private:
		int nodeID;
		int nEdges;
		std::vector<Edge> edges;	
};

#endif