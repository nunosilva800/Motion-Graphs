#include "stdafx.h"
#include "BaseApplication.h"
#include "TutorialApplication.h"


void TutorialApplication:: loadAnimationNames(){

	_assets->animNames = new std::vector<std::string>();

	_assets->animNames->push_back("idle");_assets->animCount++;
/*	_assets->animNames->push_back("frenteetras");_assets->animCount++;
	_assets->animNames->push_back("acenar");_assets->animCount++;
	_assets->animNames->push_back("andaracenar");_assets->animCount++;
	_assets->animNames->push_back("andarptras");_assets->animCount++;
	_assets->animNames->push_back("andaracenar2");_assets->animCount++;
	_assets->animNames->push_back("andarptras2");_assets->animCount++;
	_assets->animNames->push_back("idleacenar");_assets->animCount++;
	_assets->animNames->push_back("andarfrente");_assets->animCount++;
	_assets->animNames->push_back("trasacenar");_assets->animCount++;
	_assets->animNames->push_back("reclamarsair");_assets->animCount++;
	_assets->animNames->push_back("frenteacenar");_assets->animCount++;
	_assets->animNames->push_back("trasacenar2");_assets->animCount++;
	_assets->animNames->push_back("muitotempofalar");_assets->animCount++;
	_assets->animNames->push_back("direitaacenar");_assets->animCount++;
	_assets->animNames->push_back("apanharchao");_assets->animCount++;
	_assets->animNames->push_back("andarfrente2");_assets->animCount++;
	_assets->animNames->push_back("viraresqrapido");_assets->animCount++;
	_assets->animNames->push_back("trasdirrapdirlento");_assets->animCount++;
	_assets->animNames->push_back("diagonal");_assets->animCount++;
	_assets->animNames->push_back("diagonal2");_assets->animCount++;
	_assets->animNames->push_back("diagonal3");_assets->animCount++;
	_assets->animNames->push_back("volta");_assets->animCount++;
	_assets->animNames->push_back("volta2");_assets->animCount++;
	_assets->animNames->push_back("volta3");_assets->animCount++;
	_assets->animNames->push_back("esquerda");_assets->animCount++;
	_assets->animNames->push_back("direita");_assets->animCount++;
	_assets->animNames->push_back("confiante");_assets->animCount++;
	_assets->animNames->push_back("confianteesq");_assets->animCount++;
	_assets->animNames->push_back("confiantedir");_assets->animCount++;
	_assets->animNames->push_back("bruscoesq");_assets->animCount++;
	_assets->animNames->push_back("bruscodir");_assets->animCount++;
	_assets->animNames->push_back("diagonal4");_assets->animCount++;
	_assets->animNames->push_back("diagonal5");_assets->animCount++;
	_assets->animNames->push_back("volta4");_assets->animCount++;
	//_assets->animNames->push_back("frentetras2");_assets->animCount++;
	_assets->animNames->push_back("esquerda2");_assets->animCount++;*/

	_assets->_currentAnimation = _assets->animNames->begin();

}


void TutorialApplication::_loadFullScene( /*CDotScene &_scene*/) 
{
 	_assets->entity = mSceneMgr->createEntity(_assets->entityName, _assets->meshName);
	_assets->mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Node");
 	_assets->mNode->attachObject(_assets->entity );
	
	_assets->skeleton = _assets->entity->getSkeleton();
	_assets->anims = _assets->entity->getAllAnimationStates();	
}

void TutorialApplication::createScene(void)
{
	// create your scene here :)
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	
	// necessario nao sei bem pk apenas cria uma luz extra, a luz ambiente nao é suf para iluminar o material
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));						/// Create a light
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	mainlight = mSceneMgr->createLight("MainLight");
	mainlight->setType(Ogre::Light::LT_POINT);
	mainlight->setPosition(Ogre::Vector3(0, 25, 75));
	mainlight->setDiffuseColour(Ogre::ColourValue::White);
	mainlight->setSpecularColour(Ogre::ColourValue::White);
	mainlight->setAttenuation(8000,1,0.0005,0);
	mainlight->setPowerScale(5.0);
	mainlight->setCastShadows(true);

 	_assets = new MotionGraph();
 	_assets->meshName = "BodyMesh.mesh";
 	_assets->entityName = "BodyMesh";
	_assets->sceneName = "blake.scene";
	//_assets->_currentAnimation = "camfrente180";

	loadAnimationNames();

	//_loadFullScene( _scene);
	_loadFullScene();

	 	_assets->getAnimationState(*_assets->_currentAnimation)->setEnabled(true);
		_assets->getAnimationState(*_assets->_currentAnimation)->setLoop(true);

	_timeController = 0.0;

}

void BaseApplication::_frameRenderingQueued(const Ogre::FrameEvent& evt) 
{
	//_assets->_anims->getAnimationState(*_assets->_currentAnimation)->addTime(evt.timeSinceLastFrame);
	
	PointCloud* pc = new PointCloud();
	// time controler usado para iterar o tempo, para cada animaçao vai correr a animaçao ate ao fim com timesteps
	//	de 1/fps 
	if (_timeController <= _assets->getAnimationState(*_assets->_currentAnimation)->getLength()){ 	
	
	_assets->getAnimationState(*_assets->_currentAnimation)->setTimePosition(_timeController);

	Ogre::Skeleton::BoneIterator _i= _assets->getSkeleton()->getBoneIterator();
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
		if 	(_assets->_currentAnimation == 	_assets->animNames->end()){
			printf("All Done!!\n");
			_assets->_prepare_assets_done = true;
		}
		else {
			_assets->getAnimationState(*_assets->_currentAnimation)->setEnabled(true);
			_assets->getAnimationState(*_assets->_currentAnimation)->setLoop(true);
			_timeController=0;
		}	
	}
}