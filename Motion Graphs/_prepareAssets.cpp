
#include "DotScene.h"
#include "stdafx.h"
#include "BaseApplication.h"
#include "TutorialApplication.h"
#include "_Assets.h"

//junk
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

void TutorialApplication:: loadAnimationNames(){

		_assets->_animNames = new std::vector<std::string>();

		_assets->_animNames->push_back("2frente2tras");
		_assets->_animNames->push_back("2passosatras");
		_assets->_animNames->push_back("acenarvoltaratras");
		_assets->_animNames->push_back("andarcostas");
		_assets->_animNames->push_back("andarfrente");
		_assets->_animNames->push_back("andarfrente2");
		_assets->_animNames->push_back("bebado");
		_assets->_animNames->push_back("camfrente180");
	//	_assets->_animNames->push_back("caminharacenar");
		_assets->_animNames->push_back("esquerda");
		_assets->_animNames->push_back("esquerdaconfiante");
		_assets->_animNames->push_back("esquerdaconfiante2");
		_assets->_animNames->push_back("falargestos");
		_assets->_animNames->push_back("frenteconfiante");
		_assets->_animNames->push_back("frentedevagar");
		_assets->_animNames->push_back("frenteviragem180");
		_assets->_animNames->push_back("olharacenar");
		_assets->_animNames->push_back("olharvolta");
		_assets->_animNames->push_back("pegarchao");
		_assets->_animNames->push_back("props");
		_assets->_animNames->push_back("reclamar");
		_assets->_animNames->push_back("viragemdireita");
		_assets->_animNames->push_back("viragemesq");
		_assets->_animNames->push_back("viragemesqdevagar");
		_assets->_animNames->push_back("viraresq");

		_assets->_currentAnimation = _assets->_animNames->begin();

		
	
}

//Junk
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

void TutorialApplication::_loadFullScene( /*CDotScene &_scene*/) 
{
// 	_scene.parseDotScene(_assets->_sceneName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mSceneMgr);
// 	_assets->_entity = mSceneMgr->getEntity(_assets->_entityName);

 	_assets->_entity = mSceneMgr->createEntity(_assets->_entityName, _assets->_meshName);
	//_assets->_entity->getMesh()->Mar
 	_assets->mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Node", Ogre::Vector3(0.0f, 0.0f, 25.0f));
 	_assets->mNode->attachObject(_assets->_entity );

		
	_assets->_skeleton = _assets->_entity->getSkeleton();
	_assets->_anims = _assets->_entity->getAllAnimationStates();

	//_assets->_animeCount = _assets->_animeStates.
//	Ogre::AnimationState * _AnimationState = _Entity->getAnimationState(_animation_name);
		
	
}

void TutorialApplication::createScene(void)
{
	// create your scene here :)
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	
	// necessario nao sei bem pk apenas cria uma luz extra, a luz ambiente nao é suf para iluminar o material
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));						/// Create a light
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	mainlight = mSceneMgr->createLight("MainLight");
	mainlight->setType(Light::LT_POINT);
	mainlight->setPosition(Vector3(0, 25, 75));
	mainlight->setDiffuseColour(ColourValue::White);
	mainlight->setSpecularColour(ColourValue::White);
	mainlight->setAttenuation(8000,1,0.0005,0);
	mainlight->setPowerScale(5.0);
	mainlight->setCastShadows(true);


	//_loadEntityNames();

	

	//CDotScene _scene;

	//_loadSingleAnimationScene(_scene,_scene_name,_animation_name);


 	_assets = new MotionGraph();
 	_assets->_meshName = "BodyMesh.mesh";
 	_assets->_entityName = "BodyMesh";
	_assets->_sceneName = "blake.scene";
	//_assets->_currentAnimation = "camfrente180";

	loadAnimationNames();

	//_loadFullScene( _scene);
	_loadFullScene();

	 	_assets->_anims->getAnimationState(*_assets->_currentAnimation)->setEnabled(true);
		_assets->_anims->getAnimationState(*_assets->_currentAnimation)->setLoop(true);

	_timeController = 0.0;
}

void BaseApplication::_frameRenderingQueued(const Ogre::FrameEvent& evt) 
{
	//_assets->_anims->getAnimationState(*_assets->_currentAnimation)->addTime(evt.timeSinceLastFrame);
	
	PointCloud* pc = new PointCloud();
	// time controler usado para iterar o tempo, para cada animaçao vai correr a animaçao ate ao fim com timesteps
	//	de 1/fps 
	if (_timeController <= _assets->_anims->getAnimationState(*_assets->_currentAnimation)->getLength()){ 	
	
	_assets->_anims->getAnimationState(*_assets->_currentAnimation)->setTimePosition(_timeController);

	Ogre::Skeleton::BoneIterator _i= _assets->_skeleton->getBoneIterator();
 	for ( ; _i.hasMoreElements(); )
 	{
 		Ogre::Bone* bone=_i.getNext();
		// calcula as posiçoes no mundo, TODO: verificar se ta coerente
		const Ogre::Vector3& rp = _assets->mNode->_getDerivedPosition() + _assets->mNode->_getDerivedOrientation() * _assets->mNode->_getDerivedScale() * bone->_getDerivedPosition();

 		//const Ogre::Vector3& rp = bone->getPosition();
 		pc->addPoint(rp.x,rp.y,rp.z);
 
 	}

	_assets->insertPointCloud(*_assets->_currentAnimation,_timeController,pc);

		_timeController+=0.033; // TODO: use the iFPS
		//_timeController+=evt.timeSinceLastFrame;
	}
	else {
		//itera as animaçoes
		_assets->_currentAnimation++;
		if 	(_assets->_currentAnimation == 	_assets->_animNames->end()){
			printf("All Done!!\n");
			_assets->_prepare_assets_done = true;
		}
		else {
			_assets->_anims->getAnimationState(*_assets->_currentAnimation)->setEnabled(true);
			_assets->_anims->getAnimationState(*_assets->_currentAnimation)->setLoop(true);
			_timeController=0;
		}
		
	}
	
}