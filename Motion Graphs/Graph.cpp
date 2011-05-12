#include "Graph.h"

/*
 * Contructors and Destructors
 */

Graph::Graph(){
	this->entity = NULL;
	this->indexes = NULL;
	this->nNodes = 0;
}

Graph::Graph(Ogre::Entity *e){
	this->entity = e;
	this->indexes = NULL;
	this->nNodes = 0;
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


void Graph::constructGraph(Ninja motions, int nMotions, float threshold, int nCoincidents){
	if(nMotions = 0) return;

	this->initIndexes(motions,nMotions);
	
	sNinja::iterator it;
	int i;

	for(i = 0, it = motions->begin() ; it != motions->end() ; it++,i++){
		this->indexes[i][0] = this->addNode(new gNode());
		this->indexes[i][it->second.getNPointClouds()-1] = this->addNode(new gNode());
		this->getNode(this->indexes[i][0])->addEdge(new Edge(
			this->getNode(this->indexes[i][it->second.getNPointClouds()-1]), it->second.getLabel()));
	}

	dMap map = dMap(nMotions);
	map.setNSteps(nCoincidents);
	map.setThreshold(threshold);

	map.constructMap(motions, nMotions);

	for(int i = 0 ; i < map.getNRelations() ; i++){
		std::vector<int> pts1;
		std::vector<int> pts2;

		int m1 = -1, m2 = -1;
		for(i = 0, it = motions->begin() ; it != motions->end() && (m1 == -1 || m2 == -1) ; it++,i++){
			if(it->second.getLabel().compare(map.relations[i][0]) == 0) m1 = i;
			if(it->second.getLabel().compare(map.relations[i][1]) == 0) m2 = i;
		}

		int nTransitionPoints = map.getMinimuns(i,pts1,pts2);

		for(int j = 0 ; j < nTransitionPoints ; j++){
			//Cria os nodos
			if(this->indexes[m1][pts1[i]] == -1) 
				this->indexes[m1][pts1[i]] = this->addNode(new gNode());
			if(this->indexes[m2][pts2[i]] == -1)
				this->indexes[m2][pts2[i]] = this->addNode(new gNode());
			
			//Liga a transacção
			this->createTransition(map.relations[i][0], this->indexes[m1][pts1[i]], pts1[i],
								   map.relations[i][1], this->indexes[m2][pts2[i]], pts2[i], j, nCoincidents);
		}
	}

	for(i = 0, it = motions->begin() ; it != motions->end() ; it++, i++){
		for(int j = 0 ; j < it->second.getNPointClouds() ; j++){
			int sep = 1;
			if(this->indexes[i][j] != -1){
				for(int jj = j+1 ; jj < it->second.getNPointClouds() ; jj++){
					if(this->indexes[i][j] != -1){
						//TODO criar ninja a partir de motions[i] da frame i a j
						this->splitAnimation(it->first,sep,this->indexes[i][j],j,this->indexes[i][jj],jj);
						sep++;
					}
				}
			}
		}
	}
}




void Graph::splitAnimation(std::string name, int separation,
							int node1, int frame1,
							int node2, int frame2){
	
	std::stringstream ss;
	ss << name << "_" << separation << "1";
	std::string label = ss.str();
						   
	this->getNode(node1)->addEdge(new Edge(this->getNode(node2),label));

	//TODO split animations
	Ogre::Animation *animation = this->entity->getSkeleton()->getAnimation(name);
	
	float length = animation->getLength();
	int totalFrames = animation->getNodeTrack(0)->getNumKeyFrames();
	float tInit = ((float)((float)frame1 * length)) / ((float)totalFrames);
	float tEnd = ((float)((float)frame2 * length)) / ((float)totalFrames);
	float newDuration = tEnd - tInit;
	

	this->entity->getSkeleton()->createAnimation(label,newDuration);

	Ogre::Animation *newAnimation = animation->clone(label);//this->entity->getSkeleton()->getAnimation(label);
	
	//????
	newAnimation->destroyAllTracks();
	
	int numBones = this->entity->getSkeleton()->getNumBones();
	for(int i = 0 ; i < numBones ; i++){
		//TODO só isto?
		newAnimation->createNodeTrack(i,animation->getNodeTrack(i)->getAssociatedNode());
	
		//copiar todas as keyframes do bone
		for(int j = frame1, ac = 0 ; j < (frame1 + (frame2 - frame1)) ; j++, ac++){
			newAnimation->getNodeTrack(i)->createNodeKeyFrame(
				animation->getNodeTrack(i)->getNodeKeyFrame(j)->getTime() - 
				animation->getNodeTrack(i)->getNodeKeyFrame(frame1)->getTime());

			newAnimation->getNodeTrack(i)->getNodeKeyFrame(ac)->setRotation(
				animation->getNodeTrack(i)->getNodeKeyFrame(j)->getRotation());
			newAnimation->getNodeTrack(i)->getNodeKeyFrame(ac)->setScale(
				animation->getNodeTrack(i)->getNodeKeyFrame(j)->getScale());
			newAnimation->getNodeTrack(i)->getNodeKeyFrame(ac)->setTranslate(
				animation->getNodeTrack(i)->getNodeKeyFrame(j)->getTranslate());

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

void Graph::createTransition(std::string m1, int node1, int frame1,
							 std::string m2, int node2, int frame2,
							 int transiction,int range){

	std::stringstream ss;
	ss << m1 << "_" ;
	ss << m2 << "_" << transiction;
	std::string newName = ss.str();

	this->getNode(node1)->addEdge(new Edge(this->getNode(node2),newName));

	//TODO criar animação com o newName entre frame1 de m1 e frame2 de m2
	/*
		true,this->motionGraph->getMotion(
		&motions->at(map.relations[i][0]),&motions->at(map.relations[i][0]),
		pts1[j],pts2[j]-nCoincidents,pts1[j] + nCoincidents,pts2[j])));
	*/

}


void Graph::initIndexes(Ninja motions, int nMotions){
	int maxFrames = -1;

	sNinja::iterator it;

	for(it = motions->begin() ; it != motions->end() ; it++){
		if(it->second.getNPointClouds() > maxFrames)
			maxFrames = it->second.getNPointClouds();
	}

	this->indexes = (int**)malloc(sizeof(int*) * nMotions);
	
	for(int i = 0 ; i < nMotions ; i++){
		this->indexes[i] = (int*)malloc(sizeof(int) * maxFrames);
	}

	for(int i = 0 ; i < nMotions ; i++){
		for(int j = 0 ; j < maxFrames ; j++){
			this->indexes[i][j] = -1;
		}
	}
}