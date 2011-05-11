#include "Motion.h"

/*
 * Construtores
 */
Motion::Motion(){

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