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
	if(this->nPoints < i) return 0;

	return this->points[i];
}


/**************************************************************************
 * Motion
**************************************************************************/

Motion::Motion(){

}

Motion::~Motion(){

}


PointCloud *Motion::getPointCloud(int i){
	if(i > this->nClouds) return 0;

	return this->clouds[i];
}
