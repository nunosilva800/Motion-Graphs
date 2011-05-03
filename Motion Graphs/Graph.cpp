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
gNode* Graph::existNode(gNode *node){
	for(int i = 0 ; i < this->nNodes ; i++){
		if(node->getID() == this->nodes[i].getID()) return this->getNode(i);
	}
	
	return NULL;
}

/**
 * retorna o nodo na posicao pretendida ou nulo caso essa posicao nao exista
 */
gNode *Graph::getNode(int node){
	if(node >= this->nNodes) return NULL;

	return &this->nodes[node];
}


void constructGraph(Ninja **ninjas, int nNinjas){
	Motion **motions = NULL;

	if(nNinjas = 0) return;
	motions = (Motion**)malloc(sizeof(Motion*) * nNinjas);

	for(int i = 0 ; i < nNinjas ; i++){
		motions[i] = ninjas[i]->getMotion();
	}

	dMap map = dMap(nNinjas);

	if(motions) map.constructMap(motions, nNinjas);

	//TODO procurar pontos provaveis de haver transição

}
