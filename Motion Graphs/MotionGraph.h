#ifndef _MOTIONGRAPH
#define _MOTIONGRAPH
#include "stdafx.h"
#include "stdafx.h"
#include "Motion.h"
#include "Graph.h"

#include <OgreEntity.h>



class MotionGraph{
	public:
		MotionGraph();
		~MotionGraph();

		void setMeshName(std::string name){this->meshName = name;}
		void setSceneName(std::string name){this->sceneName = name;}
		void setEntityName(std::string name){this->entityName = name;}
		void setEntity(Ogre::Entity *e){this->entity = e;}

		std::string getMeshName(){return this->meshName;}
		std::string getSceneName(){return this->sceneName;}
		std::string getEntityName(){return this->entityName;}
		Ogre::Entity *getEntity(){return this->entity;}

		Ogre::SkeletonInstance *getSkeleton(){return this->entity->getSkeleton();}
		Ogre::AnimationStateSet *getAnimationStates(){return this->entity->getAllAnimationStates();}
		Ogre::AnimationState *getAnimationState(std::string name){return this->entity->getAnimationState(name);}
		
		void insertMotion(Motion *motion);
		Motion *getMotion(std::string name);

		void constructGraph(float threshold, int nCoincidents);

		void insertPointCloud(std::string animation,Ogre::Real indexFrame,PointCloud* pt);

		bool assetsPrepared(){return this->_prepare_assets_done;}


	//private:
		Graph *graph;
		std::string meshName;
		std::string sceneName;
		std::string entityName;
		std::vector<std::string>::iterator _currentAnimation;

		Ogre::SceneNode* mNode;
		Ogre::Entity *entity;
		Ogre::SkeletonInstance *skeleton;

		int animCount;
		std::vector<std::string>* animNames;
		//AnimationStateSet* _anims;
		Ogre::AnimationStateSet* anims;
		bool _prepare_assets_done;
		Ninja motions;
};

#endif