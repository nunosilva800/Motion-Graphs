#ifndef _MOTIONGRAPH
#define _MOTIONGRAPH

//#include "Graph.h"
#include "Motion.h"
#include <OgreEntity.h>
#include <map>

typedef std::map<std::string,Motion*>* Ninja;

using namespace Ogre;

class MotionGraph{
	public:
		MotionGraph();
		~MotionGraph();

		void setMeshName(std::string name){this->_meshName = name;}
		void setSceneName(std::string name){this->_sceneName = name;}
		void setEntityName(std::string name){this->_entityName = name;}
		void setEntity(Entity *e){this->_entity = e;}

		std::string getMeshName(){return this->_meshName;}
		std::string getSceneName(){return this->_sceneName;}
		std::string getEntityName(){return this->_entityName;}
		Entity *getEntity(){return this->_entity;}

		SkeletonInstance *getSkeleton(){return this->_entity->getSkeleton();}
		AnimationStateSet *getAnimationStates(){return this->_entity->getAllAnimationStates();}
		AnimationState *getAnimationState(std::string name){return this->_entity->getAnimationState(name);}
		
		void insertMotion(Motion *motion);
		Motion *getMotion(std::string name);

		void constructGraph(float threshold, int nCoincidents);

		void insertPointCloud(std::string animation,Ogre::Real indexFrame,PointCloud* pt){
			if (this->motions->find(animation) == this->motions->end()){
				this->motions->insert(pair<std::string,Motion*>(animation,new Motion()));
				}

			//this->motions->at(animation)->map_clouds->at(indexFrame) = pt;
			Motion* m= this->motions->at(animation);

			if (m->map_clouds->find(indexFrame) == m->map_clouds->end()){
				m->map_clouds->insert(pair<Ogre::Real,PointCloud*>(indexFrame,new PointCloud()));
			}

			m->map_clouds->at(indexFrame) = pt;
		}


	//	Graph *graph;
		std::string _meshName;
		std::string _sceneName;
		std::string _entityName;
		std::vector<std::string>::iterator _currentAnimation;

		SceneNode* mNode;
		
		Entity *_entity;
		SkeletonInstance *_skeleton;

		int _animCount;
		std::vector<std::string>* _animNames;
		AnimationStateSet* _anims;
		bool _prepare_assets_done;
		Ninja motions;
};

#endif