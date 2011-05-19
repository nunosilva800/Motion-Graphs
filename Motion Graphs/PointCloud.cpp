#include "stdafx.h"
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
	for(int i = 0 ; i < NPOINTS ; i++){
		float r = sqrt(pow(this->getPoint(i)->getX(),2) + pow(this->getPoint(i)->getZ(),2));
		float teta = atan(this->getPoint(i)->getX() / this->getPoint(i)->getZ());
		teta += angle;

		this->getPoint(i)->setX(r * cos(teta));
		this->getPoint(i)->setZ(r * sin(teta));
	}
}

void PointCloud::translate(float xx, float yy, float zz){
	for(int i = 0 ; i < NPOINTS ; i++){
		this->points.at(i).setX(this->points.at(i).getX() + xx);
		this->points.at(i).setY(this->points.at(i).getY() + yy);
		this->points.at(i).setZ(this->points.at(i).getZ() + zz);
	}
}
