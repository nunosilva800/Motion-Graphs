#include "PointCloud.h"

/**************************************************************************
 * Point
**************************************************************************/

Point::Point(){

}

Point::Point(float xx, float yy, float zz){
	this->setCoordinates(xx,yy,zz);
}

Point::Point(int xx, int yy, int zz){
	this->setCoordinates((float)xx,(float)yy,(float)zz);
}

Point::~Point(){

}




/**************************************************************************
 * PointCloud
**************************************************************************/

PointCloud::PointCloud(){

}

PointCloud::~PointCloud(){

}



Point *PointCloud::getPoint(int i){
	if(i >= NPOINTS) return NULL;

	return &this->points[i];
}

/*
 * axis:
 *	0 - X
 *	1 - Y
 *	2 - Z
 */
void rotate(int axis, float angle){
	
}

void translate(float xx, float yy, float zz){

}


/**************************************************************************
 * Motion
**************************************************************************/

Motion::Motion(){

}

Motion::~Motion(){

}


PointCloud *Motion::getPointCloud(int i){
	if(i > this->nClouds) return NULL;

	return &this->clouds[i];
}
