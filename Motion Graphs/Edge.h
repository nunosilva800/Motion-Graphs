#ifndef _EDGE
#define _EDGE

#include "Motion.h"

class gNode;

class Edge{
	public:
		Edge();
		Edge(gNode *d, std::string label);
		//Edge(gNode *d, std::string label, bool transition, Motion *m1, Motion *m2, int init1, int init2, int end1, int end2);
		~Edge();

		void setLabel(std::string label){this->label = label;}
		void setDestination(gNode *d){this->dest = d;}

		std::string getLabel(){return this->label;}
		gNode *getDestionation(){return this->dest;}


	private:
		gNode *dest;
		std::string label;

		//Teste
		/*bool transition;
		Motion *m1,*m2;
		int init1,init2,end1,end2;*/

};

//typedef struct le{
//	Edge *edge;
//	struct le *next;
//} *lEdge, LEdge;

#endif