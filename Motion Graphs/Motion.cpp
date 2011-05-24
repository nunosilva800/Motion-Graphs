#include "stdafx.h"
#include "Motion.h"

/*
 * Construtores
 */
Motion::Motion(){
	this->nClouds = 0;
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

void Motion::addPointCloud(PointCloud *p){
	this->clouds.push_back(*p);

}