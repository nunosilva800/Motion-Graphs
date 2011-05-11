
#include "DotScene.h"
#include "stdafx.h"
#include "BaseApplication.h"
#include "TutorialApplication.h"
#include "_Assets.h"

void TutorialApplication::_loadEntityNames() 
{
	
	_entityNames.push_back("buckle");
	_entityNames.push_back("belt");
	_entityNames.push_back("right_shoe");
	_entityNames.push_back("left_shoe");
	_entityNames.push_back("rightHand");
	_entityNames.push_back("mouthSock_neutral");
	_entityNames.push_back("teeth");
	_entityNames.push_back("tongue");
	_entityNames.push_back("neutral_brows");
	_entityNames.push_back("torso");
	_entityNames.push_back("standardTieCollar");
	_entityNames.push_back("hair2");
	_entityNames.push_back("blake_leftEye");
	_entityNames.push_back("blake_rightEye");
	_entityNames.push_back("neutral");
	_entityNames.push_back("leftHand");
	_entityNames.push_back("standardTie");
	_entityNames.push_back("buckle");
}

void TutorialApplication::_loadSingleAnimationScene( CDotScene &_scene, std::string _scene_name,std::string _animation_name) 
{
	_scene.parseDotScene(_scene_name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,  
		mSceneMgr
		);

	std::vector<string>::iterator _i;
	for(_i = _entityNames.begin();_i != _entityNames.end(); _i++){

		Entity* _Entity = mSceneMgr->getEntity(*_i);
		_entities.push_back(_Entity);

		SkeletonInstance* sk = _Entity->getSkeleton();
		_skeletons.push_back(sk);
		
		Ogre::AnimationState * _AnimationState = _Entity->getAnimationState(_animation_name);
		_animStates.push_back(_AnimationState);
		_AnimationState->setLoop(true);
		_AnimationState->setEnabled(true);

	}
} 

void TutorialApplication::_loadFullScene( CDotScene &_scene) 
{
	_scene.parseDotScene(_assets->_sceneName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mSceneMgr);
	_assets->_entity = mSceneMgr->getEntity(_assets->_entityName);

// 	_assets->_entity = mSceneMgr->createEntity(_assets->_entityName, _assets->_meshName);
// 	mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("RobotNode", Ogre::Vector3(0.0f, 0.0f, 25.0f));
// 	mNode->attachObject(_assets->_entity );

		
	_assets->_skeleton = _assets->_entity->getSkeleton();
	_assets->_anims = _assets->_entity->getAllAnimationStates();

	//_assets->_animeCount = _assets->_animeStates-
		//Ogre::AnimationState * _AnimationState = _Entity->getAnimationState(_animation_name);
		
	
}

void TutorialApplication::createScene(void)
{
	// create your scene here :)
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

	//_loadEntityNames();

// 	std::string _scene_name = "blakeacenarandar2.scene";
// 	std::string _animation_name = "acernarandar2";

	//std::string _scene_name = "blake.scene";
//	std::string _animation_name = "andar";

	//blake

	CDotScene _scene;

	//_loadSingleAnimationScene(_scene,_scene_name,_animation_name);


	_assets = new _Assets();
	_assets->_meshName = "robot.mesh";
	_assets->_entityName = "Robot";


	Ninja m;

	_loadFullScene( _scene);

	 	_assets->_anims->getAnimationState("Walk")->setEnabled(true);
		_assets->_anims->getAnimationState("Walk")->setLoop(true);

	_timeController = 0.0;
}

void BaseApplication::_frameRenderingQueued(const Ogre::FrameEvent& evt) 
{


	PointCloud pc;
 	_assets->_anims->getAnimationState("Walk")->addTime(evt.timeSinceLastFrame);


	Ogre::Skeleton::BoneIterator _i= _assets->_skeleton->getBoneIterator();
	for ( ; _i.hasMoreElements(); )
	{
		Ogre::Bone* bone=_i.getNext();
		const Ogre::Vector3& rp = bone->getPosition();
		pc.addPoint(rp.x,rp.y,rp.z);

	}


	
	/*

	PointCloud pc;


	//	if (_timeController <= (*(_animStates.begin()))->getLength()){

	int index =0;

	Point* p;
	std::vector<Ogre::AnimationState*>::iterator _i;
	for(_i = _animStates.begin();_i != _animStates.end(); _i++){
		(*_i)->addTime(evt.timeSinceLastFrame);	
		//(*_i)->setTimePosition(_timeController);	

		(*_i)->setTimePosition(2);
		const Ogre::Vector3& rp = _skeletons.at(index)->getRootBone()->getPosition();


		(*_i)->setTimePosition(6);
		const Ogre::Vector3& rpp = _skeletons.at(index)->getRootBone()->getPosition();



		Ogre::Skeleton::BoneIterator i = _skeletons.at(index)->getBoneIterator();
		for ( ; i.hasMoreElements(); )
		{



			Ogre::Bone* bone=i.getNext();
			const Ogre::Vector3& rp = bone->getPosition();
			p = new Point(rp.x,rp.y,rp.z);
			pc.addPoint(rp.x,rp.y,rp.z);

		}

		//	(*i)->setTimePosition(27);
		index++;
	}
	_timeController+=0.03; // TODO: use the iFPS
*/

}