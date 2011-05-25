#include "stdafx.h"
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
 * Adiciona o novo nodo ao grafo e atribui-lhe um ID (incremental)
 */
int Graph::addNode(gNode *node){
	node->setID(this->nNodes);
	this->nodes.push_back(*node);
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
	if(node >= this->nNodes || node < 0) return NULL;
	else return &this->nodes.at(node);
}


void Graph::constructGraph(Ninja motions, int nMotions, float threshold, int nCoincidents){
	if(nMotions == 0) return;

	this->initIndexes(motions,nMotions);
	
	sNinja::iterator it;
	int i,j;

	for(it = motions->begin() , i = 0 ; it != motions->end() ; it++, i++){
		gNode *n1 = new gNode();
		
		gNode *n2 = new gNode();
		
		Edge *e = new Edge(n2,it->first);
		n1->addEdge(e);

		int lastPos = it->second->getNPointClouds()-1;
		this->indexes[i][0] = this->addNode(n1);
		this->indexes[i][lastPos] = this->addNode(n2);
	}
	
	dMap map = dMap(nMotions);
	map.setNSteps(nCoincidents);
	map.setThreshold(threshold);

	map.constructMap(motions, nMotions);

	for(i = 0 ; i < map.getNRelations() ; i++){
		std::string m1 = *map.relations[i][0];
		std::string m2 = *map.relations[i][1];

		int index1 = -1, index2 = -1;

		for(it = motions->begin(), j = 0 ; it != motions->end() ; it++, j++){
			if(it->first.compare(m1) == 0) index1 = j;
			if(it->first.compare(m2) == 0) index2 = j;
		}

		if(index1 != -1 && index2 != -1){

			std::vector<int> min1,min2;

			int nRelations = map.getMinimuns(i,&min1,&min2);

			for(j = 0 ; j < nRelations ; j++){
				if(this->indexes[i][min1[j]] < 0 || this->indexes[i][min1[j]] > this->nNodes){
					gNode *n = new gNode();
					this->indexes[i][min1[j]] = this->addNode(n);
				}

				if(this->indexes[i][min2[j]] < 0 || this->indexes[i][min2[j]] > this->nNodes){
					gNode *n = new gNode();
					this->indexes[i][min2[j]] = this->addNode(n);
				}

				this->createTransition(m1,this->indexes[i][min1[j]],min1[j],
									   m2,this->indexes[i][min2[j]],min2[j],j,map.getNSteps());
			}
		}
	}

	int k;

	for(it = motions->begin(), i = 0 ; it != motions->end() ; it++, i++){
		int sep = 1;
		for(j = 0 ; j < it->second->getNPointClouds() ; j++){
			if(this->indexes[i][j] != -1){
				bool done = false;
				for(k = j + 1 ; k < it->second->getNPointClouds() && !done ; k++){
					if(this->indexes[i][k] != -1){
						this->splitAnimation(it->first,sep,this->indexes[i][j],j,this->indexes[i][k],k);
						sep++;
						done = true;
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
	ss << name << "_" << separation;
	std::string label = ss.str();
						   
	Edge *e = new Edge(this->getNode(node2),label);
	this->getNode(node1)->addEdge(e);

	//TODO split animations
	/*
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
	
	//int numBones = this->entity->getSkeleton()->getNumBones();
	for(int i = 0 ; i < animation->getNumNodeTracks() ; i++){
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
	*/
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

	if(node1 > 0  && node2 > 0 && node2 < this->nNodes){
		gNode *n1 = this->getNode(node1);
		gNode *n2 = this->getNode(node2);

		if(n1 && n2){
			Edge *e = new Edge(n2,newName);
			n1->addEdge(e);
			this->changeNode(node1,n1);
		}
	}

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
		if(it->second->getNPointClouds() > maxFrames)
			maxFrames = it->second->getNPointClouds();
	}
	maxFrames *= 1.5;
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

void Graph::printGraph(char *path){
	FILE *fp = NULL;
	Edge* eAux;
	gNode* gAux;
	if((fp = fopen(path,"w")) == NULL) return ;

 	fprintf(fp,"digraph G {\n");
	fprintf(fp,"subgraph cluster0{\n");
	fprintf(fp,"style=filled;\n");
	fprintf(fp,"color= red;\n");
	fprintf(fp,"node [ fillcolor =white color = black  style=filled  shape = hexagon] ; \n");

	for(int i = 0 ; i < this->nNodes ; i++){
		gAux = this->getNode(i);
		if(gAux){
			for(int j = 0 ; j < gAux->getNEdges() ; j++){
				eAux = gAux->getEdge(j);
				if(eAux){
					int id = gAux->getID();
					int idDest = 0;
					gNode *gDest = eAux->getDestionation();
					std::string anim = eAux->getLabel();
					const char *name = NULL;
					
					if(id < 0){
						id = -101010101;
					}
					if(!gDest){
						idDest = -101010101;
					}
					else{
						try{
						idDest = gDest->getID();
						}
						catch (exception &e){
							idDest = -1;
						}
						if(idDest < 0){
							idDest = -101010101;
						}
					}
					if(anim.size() > 0){
						name = anim.data();
					}
					else name = "NONAMENONAMENONAMENONAME";
					fprintf(fp,"\t\"%d\" -> \"%d\" [label = \"%s\"];\n",id,idDest,name);
																		//gAux->getID(),
																		//(eAux->getDestionation()) ? eAux->getDestionation()->getID() : -10101010101,
																		//eAux->getLabel().data());
																		//this->nodes[i].getID(),
																		//this->nodes[i].getEdge(j)->getDestionation()->getID(),
																		//this->nodes[i].getEdge(j)->getLabel().data());
				}
			}
		}
	}
	fprintf(fp,"}\n");

	fclose(fp);
}



bool Graph::removeNode(int i)
{
	nodes.erase( nodes.begin() + i);
	nNodes--;

	return true;
}

bool Graph::changeNode(int i, gNode *node)
{
	nodes[i] = *node;

	return true;
}