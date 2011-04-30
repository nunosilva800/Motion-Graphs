#ifndef _DMAP
#define _DMAP

#include "Ninja.h"

class dMap{
	public:
		dMap();
		dMap(int nMotions);
		~dMap();

		void constructMap(Motion **motions, int nMotion);
		float **compareMotions(Motion *m1, Motion m2);
		float compareFrames(PointCloud *s1, PointCloud *s2);

	private:
		float ***differenceMap;
		Motion **relations;
		int nRelations;

		void duplicateSpace();
};

#endif