#include "stdafx.h"
#include "Point3D.h"

/*
 * Construtores
 */
Point3D::Point3D(){
	this->weight = 1.0;
}

Point3D::Point3D(float xx, float yy, float zz){
	this->setCoordinates(xx,yy,zz);
	this->weight = 1.0;
}

Point3D::Point3D(int xx, int yy, int zz){
	this->setCoordinates((float)xx,(float)yy,(float)zz);
	this->weight = 1.0;
}

Point3D::~Point3D(){

}

/*
 * Metodos
 */
