#include "PointCloud.h"

/*
 * Construtores
 */

PointCloud::PointCloud(){

}

PointCloud::~PointCloud(){

}


/*
 * Metodos
 */

Point3D *PointCloud::getPoint(int i){
	if(i >= NPOINTS) return NULL;

	return &this->points[i];
}

/*
 * axis:
 *	0 - X
 *	1 - Y
 *	2 - Z
 */
void PointCloud::rotate(int axis, float angle){
	
}

void PointCloud::translate(float xx, float yy, float zz){

}
