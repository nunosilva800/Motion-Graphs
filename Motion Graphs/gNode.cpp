#include "stdafx.h"
#include "gNode.h"

/*
 * Contructors and Destructors
 */

gNode::gNode(){
	this->nEdges = 0;
	this->nodeID = -1;
}

gNode::gNode(int id){
	this->nEdges = 0;
	this->nodeID = id;
}

gNode::~gNode(){

}

/*
 * Methods
 */

int gNode::addEdge(Edge *edge){
	if(!edge) return -1;
	this->edges.push_back(*edge);
	
	this->nEdges++;
	return this->nEdges;
}

Edge *gNode::getEdge(int e){
	if(e >= this->nEdges) return NULL;

	return &this->edges[e];
}

/*Edge **gNode::getEdges(){
	Edge **edges = (Edge**)malloc(sizeof(Edge*) * this->nEdges);
	lEdge aux = this->edges;

	for(int i = 0 ; i < this->nEdges ; i++, aux = aux->next){
		edges[i] = aux->edge;
	}

	return edges;
}*/