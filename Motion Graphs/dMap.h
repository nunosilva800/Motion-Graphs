#ifndef _DMAP
#define _DMAP

#include "Motion.h"

//using namespace std;



class Graph;

class dMap{
	friend Graph;
	public:
		dMap();
		dMap(int nMotions);
		~dMap();

		void constructMap(Ninja motions, int nMotion);
		void compareMotions(Motion *m1, Motion *m2);
		float compareFrames(PointCloud *s1, PointCloud *s2);
		void calculateTransformation(PointCloud *s1, PointCloud *s2, float *teta, float *x0, float *z0);

		void setNRelations(int r){this->nRelations = r;}
		int getNRelations(){return this->nRelations;}
		void setThreshold(float t){this->threshold = t;}
		float getThreshold(){return this->threshold;}
		void setNSteps(int n){this->nSteps = n;}
		int getNSteps(){return this->nSteps;}
		

		int getMinimuns(int level, std::vector<int> m1, std::vector<int> m2);

	private:
		Ninja motions;
		float ***differenceMap;
		std::string **relations;
		int nRelations;
		int maxRelations;
		float threshold;
		int nSteps;

		void duplicateSpace();
		void calculateSums(PointCloud *s1, PointCloud *s2, float *x1_, float *x2_, float *z1_, float *z2_, float *weights);
		float calculateTeta1(PointCloud *s1, PointCloud *s2);
		float calculateTeta2(float x1_, float x2_, float z1_, float z2_, float weights);
		float calculateTeta3(PointCloud *s1, PointCloud *s2);
		float calculateTeta4(float x1_, float x2_, float z1_, float z2_, float weights);
		
		float difference(Point3D *p1, Point3D *p2){
			return sqrt(pow(p1->getX() - p2->getX(),2) + 
						pow(p1->getY() - p2->getY(),2) + 
						pow(p1->getZ() - p2->getZ(),2) );
		}
};

#endif