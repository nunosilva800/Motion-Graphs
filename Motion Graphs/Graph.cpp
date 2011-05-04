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
int Graph::addNode(gNode *node){
	this->nodes.push_back(*node);
	node->setID(this->nNodes);
	this->nNodes++;
	return node->getID();
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


void Graph::constructGraph(Ninja **ninjas, int nNinjas, float threshold, int nCoincidents){
	Motion **motions = NULL;

	if(nNinjas = 0) return;
	else motions = (Motion**)malloc(sizeof(Motion*) * nNinjas);

	this->initIndexes(ninjas,nNinjas);

	for(int i = 0 ; i < nNinjas ; i++){
		motions[i] = ninjas[i]->getMotion();
		//Constroi os nodos
		this->indexes[i][0] = this->addNode(new gNode());
		this->indexes[i][ninjas[i]->getMotion()->getNPointClouds()-1] = this->addNode(new gNode());
		this->getNode(this->indexes[i][0])->addEdge(new Edge(
			this->getNode(this->indexes[i][ninjas[i]->getMotion()->getNPointClouds()-1]), ninjas[i]
			,false,ninjas[i]->getMotion(),NULL,0,-1,ninjas[i]->getMotion()->getNPointClouds()-1,-1));
	}

	dMap map = dMap(nNinjas);
	map.setNSteps(nCoincidents);
	map.setThreshold(threshold);

	if(motions) map.constructMap(motions, nNinjas);

	for(int i = 0 ; i < map.getNRelations() ; i++){
		std::vector<int> pts1;
		std::vector<int> pts2;
		//CONFIRMAR
		int m1 = i % (nNinjas-1) + i / (nNinjas-1) - 1;
		int m2 = i % (nNinjas-1);

		int nTransitionPoints = map.getMinimuns(i,pts1,pts2);

		for(int j = 0 ; j < nTransitionPoints ; j++){
			//Cria os nodos
			if(this->indexes[m1][pts1[i]] == -1) this->indexes[m1][pts1[i]] = this->addNode(new gNode());
			if(this->indexes[m2][pts2[i]] == -1) this->indexes[m2][pts2[i]] = this->addNode(new gNode());
			//Liga a transacção
			//TODO criar Ninja de transacção
			this->getNode(this->indexes[m1][pts1[i]])->addEdge(new Edge(this->getNode(this->indexes[m2][pts2[i]]),NULL
				,true,ninjas[m1]->getMotion(),ninjas[m2]->getMotion(),pts1[j],pts2[j]-nCoincidents,pts1[j] + nCoincidents,pts2[j]));
		}
	}

	for(int i = 0 ; i < nNinjas ; i++){
		for(int j = 0 ; j < ninjas[i]->getMotion()->getNPointClouds() ; j++){
			if(this->indexes[i][j] != -1){
				for(int jj = j+1 ; jj < ninjas[i]->getMotion()->getNPointClouds() ; jj++){
					if(this->indexes[i][j] != -1){
						//TODO criar ninja a partir de ninjas[i] da frame i a j
						this->getNode(this->indexes[i][j])->
							addEdge(new Edge(this->getNode(this->indexes[i][jj]),NULL,false,ninjas[i]->getMotion(),NULL,j,-1,jj,-1));
					}
				}
			}
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


}*/


void Graph::initIndexes(Ninja **ninjas, int nNinjas){
	int maxFrames = -1;

	for(int i = 0 ; i < nNinjas ; i++){
		if(ninjas[i]->getMotion()->getNPointClouds() > maxFrames) 
			maxFrames = ninjas[i]->getMotion()->getNPointClouds();
	}

	this->indexes = (int**)malloc(sizeof(int*) * nNinjas);
	
	for(int i = 0 ; i < nNinjas ; i++){
		this->indexes[i] = (int*)malloc(sizeof(int) * maxFrames);
	}

	for(int i = 0 ; i < nNinjas ; i++){
		for(int j = 0 ; j < maxFrames ; j++){
			this->indexes[i][j] = -1;
		}
	}
}