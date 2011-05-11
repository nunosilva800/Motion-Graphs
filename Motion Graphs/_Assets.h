#ifndef __Assets_h_
#define __Assets_h_



#include "stdafx.h"
#include "BaseApplication.h"
#include <map>
#include "Ninja.h"

#include "Motion.h"

using namespace Ogre;

class _Assets {
public:

	std::string _meshName;
	std::string _sceneName;
	std::string _entityName;
	int _animCount;
	Entity* _entity;
	SkeletonInstance* _skeleton;
	std::vector<std::string> _animNames;
	AnimationStateSet* _anims;
	std::map<std::string, Ninja> _ninjas;

	
	_Assets(void){
		_ninjas = std::map<std::string, Ninja>();
	}
	virtual ~_Assets(void){}


};

#endif