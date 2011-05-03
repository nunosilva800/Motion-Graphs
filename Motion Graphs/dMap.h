#ifndef _DMAP
#define _DMAP

#include "Ninja.h"

class dMap{
	public:
		dMap();
		dMap(int nMotions);
		~dMap();

		void constructMap(Motion **motions, int nMotion);
		void compareMotions(Motion *m1, Motion *m2);
		float compareFrames(PointCloud *s1, PointCloud *s2);
		void calculateTransformation(PointCloud *s1, PointCloud *s2, float *teta, float *x0, float *z0);

	private:
		float ***differenceMap;
		Motion ***relations;
		int nRelations;
		int maxRelations;

		void duplicateSpace();
		void calculateSums(PointCloud *s1, PointCloud *s2, float *x1_, float *x2_, float *z1_, float *z2_, float *weights);
		float calculateTeta1(PointCloud *s1, PointCloud *s2);
		float calculateTeta2(float x1_, float x2_, float z1_, float z2_, float weights);
		float calculateTeta3(PointCloud *s1, PointCloud *s2);
		float calculateTeta4(float x1_, float x2_, float z1_, float z2_, float weights);
};

#endif