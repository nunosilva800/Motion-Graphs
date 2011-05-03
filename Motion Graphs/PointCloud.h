#ifndef _POINTCLOUD
#define _POINTCLOUD

#include <stdio.h>
using namespace std;

#include <vector>

class Point{
	public:
		Point();
		Point(float xx, float yy, float zz);
		Point(int xx, int yy, int zz);
		~Point();

		void setCoordinates(float xx, float yy, float zz){this->x = xx;	this->y = yy; this->z = zz;}
		
		//Coordinate get functions
		float getX(){return this->x;}
		float getY(){return this->y;}
		float getZ(){return this->z;}
		float getWeight(){return this->weight;}

		//Coordinate get functions
		void setX(float x){this->x = x;}
		void setY(float y){this->y = y;}
		void setZ(float z){this->z = z;}
		void setWeight(float w){this->weight = w;}

	private:
		float x,y,z;
		float weight;
};


#define NPOINTS 40

class PointCloud{
	//TODO
	public:
		PointCloud();
		~PointCloud();

		Point *getPoint(int i);
		void rotate(int axis, float angle);
		void translate(float xx, float yy, float zz);

	private:
		std::vector<Point> points;
		//int nPoints; // = NPOINTS?
};

class Motion{
	//TODO
	public:
		Motion();
		~Motion();

		int getNPointClouds(){return this->nClouds;}
		PointCloud *getPointCloud(int i);


	private:
		std::vector<PointCloud> clouds;
		int nClouds;
		std::string label;
};



#endif