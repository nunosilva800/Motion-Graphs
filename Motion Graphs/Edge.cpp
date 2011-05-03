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

Edge::~Edge(){

}

/*
 * Methods
 */

