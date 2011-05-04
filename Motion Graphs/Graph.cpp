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
void Graph::addNode(gNode *node){
	this->nodes.push_back(*node);
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


void Graph::constructGraph(Ninja **ninjas, int nNinjas){
	Motion **motions = NULL;

	if(nNinjas = 0) return;
	motions = (Motion**)malloc(sizeof(Motion*) * nNinjas);

	for(int i = 0 ; i < nNinjas ; i++){
		motions[i] = ninjas[i]->getMotion();

		//Constroi os nodos
		this->addNode(new gNode(i * 2));
		this->addNode(new gNode(i * 2 + 1));
		this->getNode(i * 2)->addEdge(new Edge(this->getNode(i * 2 + 1), ninjas[i]));
	}

	dMap map = dMap(nNinjas);

	if(motions) map.constructMap(motions, nNinjas);

	for(int i = 0 ; i < map.getNRelations() ; i++){
		std::vector<int> pts1;
		std::vector<int> pts2;

		int nTransitionPoints = map.getMinimuns(i,pts1,pts2);

		for(int j = 0 ; j < nTransitionPoints ; j++){
			map.relations[j][0];
		}
	}
}


/*
		e1					   e3		e4
 1-------------->2			1----->5------->2
					---->		    \ e7
								     \
 3-------------->4			3-------->6---->4
		e2						e5		e6
*/

void Graph::createTransition(Motion *m1, int mPos1, std::vector<int> tPos1, 
							 Motion *m2, int mPos2, std::vector<int> tPos2, 
							 dMap *map, int r){
	gNode *n1,*n2,*n3,*n4,*n5,*n6;
	n1 = n2 = n3 = n4 = n5 = n6 = NULL;
	Edge *e1,*e2,*e3,*e4,*e5,*e6,*e7;
	e1 = e2 = e3 = e4 = e5 = e6 = e7 = NULL;

	n1 = &this->nodes[mPos1 * 2];
	n2 = &this->nodes[mPos1 * 2 + 1];
	n3 = &this->nodes[mPos2 * 2];
	n4 = &this->nodes[mPos2 * 2 + 1];

	std::vector<int>::iterator it = tPos1.begin();
	tPos1.erase(it);


}
