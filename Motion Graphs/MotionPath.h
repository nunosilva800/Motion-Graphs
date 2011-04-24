/*
	Path
	Define a path for a character to follow
	A path is a colection of 3D Vector points
		the coordinates define the position of the character
		the magnitude defines the velocity that it should travel to the next point
		the direction defines the orientation of the character 
		(he will move always to the next point but can be looking elsewhere)

*/

#include "stdafx.h"
#include "BaseApplication.h"

class MotionPath 
{
public:
	MotionPath(void);
	~MotionPath(void);


protected:
	// add a point to the path
	virtual void add();
	// remove a point from the path
	virtual void del();
	// get a point
	virtual void get();
	// clear the path
	virtual void clear();


private:
	//Ogre::Vector2 m_vec;
	std::vector<Ogre::Vector3> m_path;
	
};
