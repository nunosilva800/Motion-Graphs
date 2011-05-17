/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
 */
#include "stdafx.h"
#include "TutorialApplication.h"


//-------------------------------------------------------------------------------------

TutorialApplication::TutorialApplication(void) {
}
//-------------------------------------------------------------------------------------

TutorialApplication::~TutorialApplication(void) {
}

//-------------------------------------------------------------------------------------

void TutorialApplication::createCamera(void) {
    // create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");
    // set its position, direction  
    mCamera->setPosition(Ogre::Vector3(0, 10, 500));
    mCamera->lookAt(Ogre::Vector3(0, 0, 0));
    // set the near clip distance
    mCamera->setNearClipDistance(5);

    mCameraMan = new OgreBites::SdkCameraMan(mCamera); // create a default camera controller
}
//-------------------------------------------------------------------------------------

void TutorialApplication::createViewports(void) {
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------

void TutorialApplication::createScene(void) {

    // TODO: disable frustum culling
    //Ogre::Frustum * noCulling = mCamera->getCullingFrustum()->;
    //noCulling->setFOVy(Ogre::Radian(360));
    //mCamera->setCullingFrustum(noCulling);

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    MODEL = 0;

    // Create the scene node for the model
    //    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ModelNode");
    //
    //    // add the model
    //    switch (MODEL) {
    //        case 0:
    //            mEntity = mSceneMgr->createEntity("Jaiqua", "jaiqua.mesh");
    //            break;
    //    }
    //
    //    mNode->attachObject(mEntity);

    // this node if for the rest of the objects
    Ogre::SceneNode *node;

    // create the light
    Ogre::Light *light = mSceneMgr->createLight("Light1");
    light->setType(Ogre::Light::LT_POINT);
    light->setPosition(Ogre::Vector3(0, 250, 0));
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue::White);

    // Create the scene cam node
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1", Ogre::Vector3(0, 50, 0));

    // Make it look towards the model
    //node->yaw(Ogre::Degree(-45));
    node->pitch(Ogre::Degree(-90));

    // Create the pitch node
    node = node->createChildSceneNode("PitchNode1");
    node->attachObject(mCamera);

    // create a plane (the ground)
    mPlane = new Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            *mPlane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

    Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);

    entGround->setMaterialName("Examples/Rockwall");
    entGround->setCastShadows(false);

    //manually define a path
    mPath = new MotionPath();

    // Set initial point
    mPath->add(Ogre::Vector3(0, 1, 0));
    //    mPath->add(Ogre::Vector3(0, 1, 0));
    //    mPath->add(Ogre::Vector3(5, 1, 0));
    //    mPath->add(Ogre::Vector3(5, 1, 5));
    //    mPath->add(Ogre::Vector3(0, 1, 5));
    //    mPath->add(Ogre::Vector3(0, 1, 0));

    // add the path to the walklist
    for (int i = 0; i < mPath->size(); i++) {
        mWalkList.push_back(mPath->get(i));
    }

    // define a spline
    // http://www.ogre3d.org/docs/api/html/classOgre_1_1SimpleSpline.html#_details
    //spline = new Ogre::SimpleSpline();

    //create a line to show the desired path
    lines = new DynamicLines(Ogre::RenderOperation::OT_LINE_STRIP);
    for (int i = 0; i < mPath->size(); i++) {
        lines->addPoint(mPath->get(i));
    }
    lines->update();
    Ogre::SceneNode *linesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("lines");
    linesNode->attachObject(lines);

    // create a line to show the path done by the char
    lines_path_done = new DynamicLines(Ogre::RenderOperation::OT_LINE_STRIP);

    //TODO change color of the line
    /*lines_path_done->getMaterial()->setAmbient(Ogre::ColourValue(1,0,0));
    lines_path_done->getMaterial()->setDiffuse(Ogre::ColourValue(1,0,0));
    lines_path_done->getMaterial()->setCullingMode(Ogre::CullingMode::CULL_NONE);*/

    lines_path_done->update();
    Ogre::SceneNode *linesNode_path_done = mSceneMgr->getRootSceneNode()->createChildSceneNode("lines_path_done");
    linesNode_path_done->attachObject(lines_path_done);

}

bool TutorialApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    // left mouse button pressed
    if (id == OIS::MB_Left) {

        // get window height and width
        Ogre::Real screenWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
        Ogre::Real screenHeight = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();

        // convert to 0-1 offset
        Ogre::Real offsetX = arg.state.X.abs / screenWidth;
        Ogre::Real offsetY = arg.state.Y.abs / screenHeight;

        // set up the ray
        Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(offsetX, offsetY);

        // check if the ray intersects our plane
        // intersects() will return whether it intersects or not (the bool value) and
        // what distance (the Real value) along the ray the intersection is
        std::pair<bool, Ogre::Real> result = mouseRay.intersects(*mPlane);

        if (result.first) {
            // if the ray intersect the plane, we have a distance value
            // telling us how far from the ray origin the intersection occurred.
            // the last thing we need is the point of the intersection.
            // Ray provides us getPoint() function which returns a point
            // along the ray, supplying it with a distance value.

            // get the point where the intersection is
            Ogre::Vector3 point = mouseRay.getPoint(result.second);

            printf("Mouse pressed\n");
            printf("X: %f  Y: %f  Z: %f\n", point.x, point.y++, point.z);
            mPath->add(point);
            //spline->addPoint(point);
            mWalkList.push_back(point);
            // splines->update();
            lines->addPoint(point);
            lines->update();
        }

    }

    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    mCameraMan->injectMouseDown(arg, id);

    return true;
}


bool TutorialApplication::keyPressed(const OIS::KeyEvent &arg) {

    if (arg.key == OIS::KC_SPACE) // toggle visibility of advanced frame stats
    {

        state = CALC_AVATAR_PATH;
		anim_state = AVATAR_ANIM_IN_CALC;

        // TODO: Aceder aqui ao grafo 
        mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ModelNode");
        //mEntity = mSceneMgr->createEntity("Jaiqua", "jaiqua.mesh");        
        mEntity = mSceneMgr->createEntity("Jaiqua", "BodyMesh.mesh");
        mNode->attachObject(mEntity);
        //mAnimationState = mEntity->getAnimationState("andarfrente");
        //mAnimationState->setLoop(true);
        //mAnimationState->setEnabled(true);

		// Get all animations
        AnimSSet = mEntity->getAllAnimationStates();
        assIte = &AnimSSet->getAnimationStateIterator();

		mAnimationState = assIte->getNext();
		totalLenght = mAnimationState->getLength();
        mAnimationState->setEnabled(true);
		mAnimationState->setLoop(false);
        // Clean old frame positions
        framePositionCollection.clear();
    }

    return BaseApplication::keyPressed(arg);

}

void TutorialApplication::createFrameListener(void) {
    BaseApplication::createFrameListener();

    // Set default values for variables
    //    mWalkSpeed = 100.0f;
    //    mDirection = Ogre::Vector3::ZERO;
    //
    //    // Set idle animation
    //    switch (MODEL) {
    //        case 0:
    //            mAnimationState = mEntity->getAnimationState("Sneak");
    //            break;
    //    }
    //
    //    mAnimationState->setLoop(true);
    //    mAnimationState->setEnabled(true);


}

//double TutorialApplication::errorFunc(double w, double e)
//{
//	// P			-> defined path (our mPath)
//	// P'			-> actual path
//	// w[i]			-> ith frame
//	// s(w[i])		-> arc-length of frame i
//	// P(s(w[i]))	-> point in P at arc-length of frame i
//	// error function is the sum of the squared distances over all frames :
//
//	//    for(int i = 0; i < nframes; i++)
//	//    {
//	//            error += pow ( P'(s(e[i])) - P(s(e[i])) , 2 );
//	//
//	//    }
//
//
//
//
//
//}

// checks if the walklist has points to go to

bool TutorialApplication::nextLocation(void) {
    if (mWalkList.empty()) return false;

    mDestination = mWalkList.front(); // this gets the front of the deque
    mWalkList.pop_front(); // this removes the front of the deque

    mDirection = mDestination - mNode->getPosition();
    mDistance = mDirection.normalise();

    return true;
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent &evt) {

    if (state == CALC_AVATAR_PATH) 
	{

        // Run all animations of the model
           
        // If the current time lenght <= total time lenght 
        if(mAnimationState->getTimePosition() < totalLenght)
		{
            // Get current position
            Ogre::Vector3 currentPos = mEntity->getSkeleton()->getRootBone()->getPosition();
            framePositionCollection.push_back(currentPos);
            mAnimationState->addTime(evt.timeSinceLastFrame);
			//force animation update
			mEntity->_updateAnimation();
        }

        if(mAnimationState->getTimePosition() == totalLenght)
		{ 
			Ogre::Vector3 tmpVect = framePositionCollection.front();
			Ogre::Real arcLenght = 0;
			// Calculate the total arclenght
			while(!framePositionCollection.empty()){
				framePositionCollection.pop_front();
				arcLenght += (framePositionCollection.front() - tmpVect).length();
				tmpVect = framePositionCollection.front(); 
			}
			printf("Total ArcLenght is %f\n", arcLenght);
			anim_state = AVATAR_ANIM_DONE;
        }
		
		if(anim_state == AVATAR_ANIM_DONE)
		{
			if (assIte->hasMoreElements())
			{
				// get next animation 
				mAnimationState = assIte->getNext();
				mAnimationState->setEnabled(true);
				mAnimationState->setLoop(FALSE);
				framePositionCollection.clear();
				totalLenght = mAnimationState->getLength();
				anim_state = AVATAR_ANIM_IN_CALC;
			}
			else state = SHOW_AVATAR_PATH;
		}
	
	}            

	//mAnimationState->addTime(evt.timeSinceLastFrame);
    return BaseApplication::frameRenderingQueued(evt);
}



#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else

    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch (Ogre::Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                    e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif








