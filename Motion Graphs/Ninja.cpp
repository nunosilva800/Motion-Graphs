#include "Ninja.h"

/*
 * Contructors and Destructors
 */

Ninja::Ninja(){
	Ogre::Animation("",0);
}

Ninja::~Ninja(){

}

/*
 * Methods
 */

bool Ninja::addMotion(Motion *motion){
	if(this->motion) ;//TODO e se já existir uma motion?
	
	this->motion = motion;

	return true;
}

Motion *Ninja::getMotion(){
	return (this->motion) ? this->motion : NULL ;
}