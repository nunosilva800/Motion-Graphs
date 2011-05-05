#ifndef _EDGE
#define _EDGE

#include "Ninja.h"

class gNode;

class Edge{
	public:
		Edge();
		Edge(gNode *d, Ninja *n);
		Edge(gNode *d, Ninja *n,bool transition, Motion *m1, Motion *m2, int init1, int init2, int end1, int end2);
		~Edge();

		void setNinja(Ninja *ninja){this->ninja = ninja;}
		void setDestination(gNode *d){this->dest = d;}

		Ninja *getNinja(){return this->ninja;}
		gNode *getDestionation(){return this->dest;}

		Ogre::String getLabel(){return this->label;}
		void setLabel(Ogre::String label){this->label = label;}

	private:
		gNode *dest;
		Ogre::String label;
		Ninja *ninja;

		//Teste
		bool transition;
		Motion *m1,*m2;
		int init1,init2,end1,end2;

};

//typedef struct le{
//	Edge *edge;
//	struct le *next;
//} *lEdge, LEdge;

#endif