#include "gNode.h"

/*
 * Contructors and Destructors
 */

gNode::gNode(){
	this->nEdges = 0;
	this->edges = NULL;
}

gNode::~gNode(){
	lEdge aux = this->edges;

	for(int i = 0 ; i < nEdges; i++){
		aux = aux->next;
		free(this->edges->edge);
		free(this->edges);
		this->edges = aux;
	}
}

/*
 * Methods
 */

int gNode::addEdge(Edge *edge){
	lEdge newEdge = (lEdge)malloc(sizeof(LEdge));

	newEdge->edge = edge;
	newEdge->next = this->edges;
	this->edges = newEdge;

	this->nEdges++;
}

Edge **gNode::getEdges(){
	Edge **edges = (Edge**)malloc(sizeof(Edge*) * this->nEdges);
	lEdge aux = this->edges;

	for(int i = 0 ; i < this->nEdges ; i++, aux = aux->next){
		edges[i] = aux->edge;
	}

	return edges;
}