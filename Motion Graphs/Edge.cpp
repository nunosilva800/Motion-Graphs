#include "Edge.h"

/*
 * Contructors and Destructors
 */

Edge::Edge(){
	this->dest = NULL;
	this->ninja = NULL;
}

Edge::Edge(gNode *d, Ninja *n){
	this->dest = d;
	this->ninja = n;
}

Edge::Edge(gNode *d, Ninja *n,bool transition, Motion *m1, Motion *m2, int init1, int init2, int end1, int end2){
	this->dest = d;
	this->ninja = n;
	this->transition = transition;
	this->m1 = m1;
	this->m2 = m2;
	this->init1 = init1;
	this->init2 = init2;
	this->end1 = end1;
	this->end2 = end2;
}

Edge::~Edge(){

}

/*
 * Methods
 */

