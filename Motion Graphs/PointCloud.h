#ifndef _POINTCLOUD
#define _POINTCLOUD

#include <stdio.h>
#include "Point.h"

using namespace std;

#include <vector>

#define NPOINTS 40

class PointCloud{
	public:
		PointCloud();
		~PointCloud();

		Point3D *getPoint(int i);
		void rotate(int axis, float angle);
		void translate(float xx, float yy, float zz);

	private:
		std::vector<Point3D> points;
		//int nPoints; // = NPOINTS?
};

#endif