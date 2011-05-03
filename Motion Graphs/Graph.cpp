#include "Graph.h"

/*
 * Contructors and Destructors
 */

Graph::Graph(){

}

Graph::~Graph(){

}

/*
 * Methods
 */

/**
 * Adiciona o novo nodo ao grafo
 */
void Graph::addNode(gNode node){
	this->nodes.push_back(node);
}

/**
 * Verifica se aquele nodo já está introduzido no grafo
 * @return NULL caso não exista ou retorna o próprio nodo
 */
gNode* Graph::existNode(gNode node){

	return NULL;
}