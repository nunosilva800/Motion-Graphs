#pragma once

#include "Point3D.h"

class PointCloud
{
public:
	PointCloud(void);
	~PointCloud(void);

	Point3D *getPoint(int i);
	void rotate(int axis, float angle);
	void translate(float xx, float yy, float zz);
	void addPoint(float xx, float yy, float zz){
		points.push_back(Point3D(xx,yy,zz));
	}

private:
	std::vector<Point3D> points;
	//int nPoints; // = NPOINTS?
};

