#ifndef _EDGE
#define _EDGE

#include "Ninja.h"

class gNode;

class Edge{
	public:
		Edge();
		Edge(gNode *d, Ninja *n);
		~Edge();

		void setNinja(Ninja *ninja){this->ninja = ninja;}
		void setDestination(gNode *d){this->dest = d;}

		Ninja *getNinja(){return this->ninja;}
		gNode *getDestionation(){return this->dest;}

	private:
		gNode *dest;
		Ninja *ninja;
};

//typedef struct le{
//	Edge *edge;
//	struct le *next;
//} *lEdge, LEdge;

#endif