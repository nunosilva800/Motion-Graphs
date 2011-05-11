/*------------------------------------------------------------------------------
*  DotScene.h
*
*  This file provides some methods for the parsing of a .scene file, with
*  support for userData.
*------------------------------------------------------------------------------*/

#ifndef DOT_SCENE_H
#define DOT_SCENE_H

#include <vector>
#include <string>

#include "Ogre.h"
#include "../tools/tinyxml/tinyxml.h"

using namespace std;
using namespace Ogre;

class nodeProperty
{
public:
	string nodeName;
	string propertyNm;
	string valueName;
	string typeName;

	nodeProperty(string node,string propertyName,string value,string type)
	{
		nodeName = node;
		propertyNm = propertyName;
		valueName = value;
		typeName = type;
	}
};

class CDotScene
{
public:
	CDotScene() : mSceneMgr(0) {}
	virtual ~CDotScene() {}

	void parseDotScene( const string &SceneName, const string& groupName, SceneManager *yourSceneMgr, SceneNode *pAttachNode = NULL, string sPrependNode = "");
	string getProperty(string ndNm, string prop);

	std::vector<nodeProperty> nodeProperties;

protected:
	void processNode(TiXmlElement *XMLNode, SceneNode *pAttach);
	SceneManager *mSceneMgr;
	string m_sPrependNode;
};

#endif
