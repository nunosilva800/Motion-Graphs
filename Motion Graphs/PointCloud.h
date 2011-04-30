#ifndef _POINTCLOUD
#define _POINTCLOUD


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

		//Coordinate get functions
		void setX(float x){this->x = x;}
		void setY(float y){this->y = y;}
		void setZ(float z){this->z = z;}

	private:
		float x,y,z;
};


class PointCloud{
	//TODO
	public:
		PointCloud();
		~PointCloud();

	private:
		Point *p;
		int nPoint;
};

class Motion{
	//TODO
	public:
		Motion();
		~Motion();

	private:
		PointCloud * clouds;
		int nClouds;
		char * label;
};



#endif