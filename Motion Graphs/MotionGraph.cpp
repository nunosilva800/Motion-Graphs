#include "MotionGraph.h"

/*
 * Contructors and Destructors
 */
MotionGraph::MotionGraph(){
	this->graph = NULL;
}

MotionGraph::~MotionGraph(){
	
}


/*
 * Metodos
 */

Motion *MotionGraph::getMotion(std::string name){
	return &this->motions->at(name);
}

void MotionGraph::insertMotion(Motion *motion){
	this->motions->insert(pair<std::string,Motion>(motion->getLabel(),*motion));
	//this->animNames.push_back(motion->getLabel());
	//this->animCount++;
}

void MotionGraph::constructGraph(float threshold, int nCoincidents){
	if(this->animCount == 0) return;
	
	this->graph = new Graph(this->entity);

	this->graph->constructGraph(this->motions,this->animCount,threshold,nCoincidents);

	Ogre::StringVector strv = this->entity->getAnimableValueNames();

	for(int i = 0 ; i < strv.size() ; i++){
		this->animCount++;
		this->animNames.push_back(strv[0]);
	}

}