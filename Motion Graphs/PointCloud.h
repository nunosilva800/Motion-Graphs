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


class PointCloud{
	//TODO
	public:
		PointCloud();
		~PointCloud();

		int getNPoints(){return this->nPoints;}
		Point *getPoint(int i);

	private:
		Point **points;
		int nPoints;
};

class Motion{
	//TODO
	public:
		Motion();
		~Motion();

		int getNPointClouds(){return this->nClouds;}
		PointCloud *getPointCloud(int i);


	private:
		PointCloud **clouds;
		int nClouds;
		char * label;
};



#endif