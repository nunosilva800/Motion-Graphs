#ifndef _NINJA
#define _NINJA

#include "OgreAnimation.h"
#include "PointCloud.h"

using namespace Ogre;

class Ninja : public Animation{
	public:
		//Ninja();
		Ninja(Ogre::String &name, Ogre::Real d, Motion *m1);
		~Ninja();

		bool addMotion(Motion *motion);
		Motion *getMotion();


	private:
		Motion *motion;

};



#endif