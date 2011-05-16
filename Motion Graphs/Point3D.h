#ifndef _POINT
#define _POINT

class Point3D{
public:
	Point3D();
	Point3D(float xx, float yy, float zz);
	Point3D(int xx, int yy, int zz);
	~Point3D();

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

#endif