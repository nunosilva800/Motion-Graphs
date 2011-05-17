#include "MotionGraph.h"

/*
 * Contructors and Destructors
 */
MotionGraph::MotionGraph(){
	this->graph = NULL;
	this->motions = new std::map<std::string,Motion*>();
	_prepare_assets_done = false;
}

MotionGraph::~MotionGraph(){
	
}


/*
 * Metodos
 */

Motion *MotionGraph::getMotion(std::string name){
	return this->motions->at(name);
}

void MotionGraph::insertMotion(Motion *motion){
	this->motions->insert(pair<std::string,Motion*>(motion->getLabel(),motion));
	//this->animNames.push_back(motion->getLabel());
	//this->animCount++;
}

void MotionGraph::constructGraph(float threshold, int nCoincidents){
	if(this->animCount == 0) return;
	
	this->graph = new Graph(this->entity);

	this->graph->constructGraph(this->motions,this->animCount,threshold,nCoincidents);

	Ogre::StringVector strv = this->entity->getAnimableValueNames();

	for(unsigned int i = 0 ; i < strv.size() ; i++){
		this->animCount++;
		this->animNames->push_back(strv[0]);
	}

}

void MotionGraph::insertPointCloud(std::string animation,Ogre::Real indexFrame,PointCloud* pt){
	if (this->motions->find(animation) == this->motions->end()){
		this->motions->insert(pair<std::string,Motion*>(animation,new Motion()));
	}

	//this->motions->at(animation)->map_clouds->at(indexFrame) = pt;
	Motion* m= this->motions->at(animation);

	if (m->map_clouds->find(indexFrame) == m->map_clouds->end()){
		m->map_clouds->insert(pair<Ogre::Real,PointCloud*>(indexFrame,new PointCloud()));
	}

	m->map_clouds->at(indexFrame) = pt;
}