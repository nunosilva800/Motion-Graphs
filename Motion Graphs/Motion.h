#pragma once

#include "PointCloud.h"

#define NPOINTS 40


class Motion
{
public:


	int getNPointClouds(){return this->nClouds;}
	PointCloud *getPointCloud(int i);

	std::map<Ogre::Real,PointCloud> clouds; //indexado pelas keyframes
	int nClouds;
	std::string label;


	Motion(void);
	~Motion(void);


	
};

