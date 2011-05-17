#include "Motion.h"

/*
 * Construtores
 */
Motion::Motion(){
	map_clouds = new std::map<Ogre::Real,PointCloud*>();
}

Motion::~Motion(){

}


/*
 * Metodos
 */

PointCloud *Motion::getPointCloud(int i){
	if(i > this->nClouds) return NULL;

	return &this->clouds[i];
}