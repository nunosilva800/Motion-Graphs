#include "stdafx.h"
#include "BaseApplication.h"
#include "TutorialApplication.h"


void TutorialApplication:: loadAnimationNames(){

	_assets->animNames = new std::vector<std::string>();

	//_assets->animNames->push_back("idle");_assets->animCount++;
	//_assets->animNames->push_back("frenteetras");_assets->animCount++;
	//_assets->animNames->push_back("acenar");_assets->animCount++;
	//_assets->animNames->push_back("andaracenar");_assets->animCount++;
	//_assets->animNames->push_back("andarptras");_assets->animCount++;
	//_assets->animNames->push_back("andaracenar2");_assets->animCount++;
	//_assets->animNames->push_back("andarptras2");_assets->animCount++;
	//_assets->animNames->push_back("idleacenar");_assets->animCount++;
	//_assets->animNames->push_back("andarfrente");_assets->animCount++;
	//_assets->animNames->push_back("trasacenar");_assets->animCount++;
	//_assets->animNames->push_back("reclamarsair");_assets->animCount++;
	//_assets->animNames->push_back("frenteacenar");_assets->animCount++;
	//_assets->animNames->push_back("trasacenar2");_assets->animCount++;
	//_assets->animNames->push_back("muitotempofalar");_assets->animCount++;
	//_assets->animNames->push_back("direitaacenar");_assets->animCount++;
	//_assets->animNames->push_back("apanharchao");_assets->animCount++;
	_assets->animNames->push_back("andarfrente2");_assets->animCount++;
	//_assets->animNames->push_back("viraresqrapido");_assets->animCount++;
	//_assets->animNames->push_back("trasdirrapdirlento");_assets->animCount++;
	//_assets->animNames->push_back("diagonal");_assets->animCount++;
	//_assets->animNames->push_back("diagonal2");_assets->animCount++;
	//_assets->animNames->push_back("diagonal3");_assets->animCount++;
	//_assets->animNames->push_back("volta");_assets->animCount++;
	//_assets->animNames->push_back("volta2");_assets->animCount++;
	//_assets->animNames->push_back("volta3");_assets->animCount++;
	//_assets->animNames->push_back("esquerda");_assets->animCount++;
	//_assets->animNames->push_back("direita");_assets->animCount++;
	//_assets->animNames->push_back("confiante");_assets->animCount++;
	//_assets->animNames->push_back("confianteesq");_assets->animCount++;
	//_assets->animNames->push_back("confiantedir");_assets->animCount++;
	//_assets->animNames->push_back("bruscoesq");_assets->animCount++;
	//_assets->animNames->push_back("bruscodir");_assets->animCount++;
	//_assets->animNames->push_back("diagonal4");_assets->animCount++;
	//_assets->animNames->push_back("diagonal5");_assets->animCount++;
	//_assets->animNames->push_back("volta4");_assets->animCount++;
	////_assets->animNames->push_back("frentetras2");_assets->animCount++;
	//_assets->animNames->push_back("esquerda2");_assets->animCount++;

    //jaiqua stuff
    //_assets->animNames->push_back("Sneak");_assets->animCount++;

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

 //   _assets->meshName = "jaiqua.mesh";
 //	_assets->entityName = "jaiqua";
	//_assets->sceneName = "jaiqua.scene";

	//_assets->_currentAnimation = "camfrente180";

	loadAnimationNames();

	//_loadFullScene( _scene);
	_loadFullScene();

	 	_assets->getAnimationState(*_assets->_currentAnimation)->setEnabled(true);
		_assets->getAnimationState(*_assets->_currentAnimation)->setLoop(true);

	_timeController = 0.0;




	/*** from path sysnthesis ***/


	// this node is for the rest of the objects
    Ogre::SceneNode *node;

    // create the light
	/*
    Ogre::Light *light = mSceneMgr->createLight("Light1");
	light->setType(Ogre::Light::LT_POINT);
    light->setPosition(Ogre::Vector3(0, 100, 0));
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue::White);
	*/

    // Create the scene cam node
    //node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1");

    //// Make it look towards the model
    //node->yaw(Ogre::Degree(-45));

    //// Create the pitch node
    //node = node->createChildSceneNode("PitchNode1");
    //node->attachObject(mCamera);

    //// create a plane (the ground)
    mPlane = new Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            *mPlane, 500, 500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

    Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);

    entGround->setMaterialName("Examples/Rockwall");
    entGround->setCastShadows(false);



	/*** Path Synthesis specific stuff starts here ***/

	mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ModelNode", Ogre::Vector3(0,0,0));      
    mEntity = mSceneMgr->createEntity("blake", "BodyMesh.mesh");
    mNode->attachObject(mEntity);

    // Set initial point on the user defined path
	mWalkList.push_back(Ogre::Vector3(0, 1, 0));

    // define a spline
    // http://www.ogre3d.org/docs/api/html/classOgre_1_1SimpleSpline.html#_details
    //spline = new Ogre::SimpleSpline();

    //create a line to show the desired path
    lines = new DynamicLines(Ogre::RenderOperation::OT_LINE_STRIP);
    for (int i = 0; i < mWalkList.size(); i++) {
        lines->addPoint(mWalkList[i]);
    }
    lines->update();
    Ogre::SceneNode *linesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("lines");
    linesNode->attachObject(lines);

    // create a line to show the path done by the char
    lines_path_done = new DynamicLines(Ogre::RenderOperation::OT_LINE_STRIP);

	state = SET_USER_PATH;

    //TODO change color of the line
    /*lines_path_done->getMaterial()->setAmbient(Ogre::ColourValue(1,0,0));
    lines_path_done->getMaterial()->setDiffuse(Ogre::ColourValue(1,0,0));
    lines_path_done->getMaterial()->setCullingMode(Ogre::CullingMode::CULL_NONE);*/

    //lines_path_done->update();
    //Ogre::SceneNode *linesNode_path_done = mSceneMgr->getRootSceneNode()->createChildSceneNode("lines_path_done");
    //linesNode_path_done->attachObject(lines_path_done);

}

void BaseApplication::_frameRenderingQueued(const Ogre::FrameEvent& evt) 
{
	//_assets->_anims->getAnimationState(*_assets->_currentAnimation)->addTime(evt.timeSinceLastFrame);
	
	//PointCloud* pc = new PointCloud();
	//// time controler usado para iterar o tempo, para cada animaçao vai correr a animaçao ate ao fim com timesteps
	////	de 1/fps 
	//if (_timeController <= _assets->getAnimationState(*_assets->_currentAnimation)->getLength()){ 	
	//
	//_assets->getAnimationState(*_assets->_currentAnimation)->setTimePosition(_timeController);

	//Ogre::Skeleton::BoneIterator _i= _assets->getSkeleton()->getBoneIterator();
 //	for ( ; _i.hasMoreElements(); )
 //	{
 //		Ogre::Bone* bone=_i.getNext();
	//	// calcula as posiçoes no mundo, TODO: verificar se ta coerente
	//	const Ogre::Vector3& rp = _assets->mNode->_getDerivedPosition() + _assets->mNode->_getDerivedOrientation() * _assets->mNode->_getDerivedScale() * bone->_getDerivedPosition();

 //		//const Ogre::Vector3& rp = bone->getPosition();
 //		pc->addPoint(rp.x,rp.y,rp.z);
 //
 //	}

	//_assets->insertPointCloud(*_assets->_currentAnimation,_timeController,pc);

	//	_timeController+=0.1; // TODO: use the iFPS2		//_timeController+=evt.timeSinceLastFrame;
	//}
	//else {
	//	//itera as animaçoes
	//	
	//	_assets->_currentAnimation++;
	//	if 	(_assets->_currentAnimation == 	_assets->animNames->end()){
	//		printf("All Done!!\n");
	//		_assets->_prepare_assets_done = true;
	//	}
	//	else {
	//		_assets->getAnimationState(*_assets->_currentAnimation)->setEnabled(true);
	//		_assets->getAnimationState(*_assets->_currentAnimation)->setLoop(true);
	//		_timeController=0;
	//	}	
	//}
}