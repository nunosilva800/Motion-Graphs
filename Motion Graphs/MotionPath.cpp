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
	//m_path = new std::vector<Ogre::Vector3>();
	
}

MotionPath::~MotionPath(void)
{
	m_path.clear();
}

int MotionPath::size(void)
{
	return m_path.size();
}

Ogre::Vector3 MotionPath::get(int i)
{
	return m_path[i];
}

void MotionPath::add(Ogre::Vector3 p)
{
	m_path.push_back(p);
}

void MotionPath::clear()
{
	m_path.clear();
}

