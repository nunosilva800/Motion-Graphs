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


/**************************************************************************
 * Motion
**************************************************************************/

Motion::Motion(){

}

Motion::~Motion(){

}