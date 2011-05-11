#include "StdAfx.h"
#include "Motion.h"


Motion::Motion(void)
{
}


Motion::~Motion(void)
{
}


PointCloud *Motion::getPointCloud(int i){
	if(i > this->nClouds) return NULL;

	return &(this->clouds[i]);
}
