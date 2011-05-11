#ifndef _MOTION
#define _MOTION

#include "PointCloud.h"


class Motion{
	public:
		Motion();
		~Motion();

		int getNPointClouds(){return this->nClouds;}
		PointCloud *getPointCloud(int i);
		std::string getLabel(){return this->label;}

		void setPointClouds(std::vector<PointCloud> pClouds, int nClouds){this->clouds = pCloud;this->nClouds = nClouds;}
		void setNClouds(int nClouds){this->nClouds = nClouds;}
		void setLabel(std::string label){this->label = label;}


	private:
		std::vector<PointCloud> clouds;
		int nClouds;
		std::string label;
};

#endif