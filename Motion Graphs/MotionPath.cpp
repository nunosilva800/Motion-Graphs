/*
	Path
	Define a path for a character to follow

*/

#include "stdafx.h"
#include "MotionPath.h"


MotionPath::MotionPath(void)
{
	//m_vec = new Ogre::Vector2();
	//m_path = new std::vector<Ogre::Vector2>();

}

MotionPath::~MotionPath(void)
{
	m_path.clear();
}

