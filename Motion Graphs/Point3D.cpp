#include "Point3D.h"

/*
 * Construtores
 */
Point3D::Point3D(){

}

Point3D::Point3D(float xx, float yy, float zz){
	this->setCoordinates(xx,yy,zz);
}

Point3D::Point3D(int xx, int yy, int zz){
	this->setCoordinates((float)xx,(float)yy,(float)zz);
}

Point3D::~Point3D(){

}

/*
 * Metodos
 */
