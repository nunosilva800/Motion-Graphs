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
    mCamera->setPosition(Ogre::Vector3(0, 25, 50));
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

    mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

    // this node is for the rest of the objects
    Ogre::SceneNode *node;

    // create the light
    Ogre::Light *light = mSceneMgr->createLight("Light1");
	light->setType(Ogre::Light::LT_POINT);
    light->setPosition(Ogre::Vector3(0, 100, 0));
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue::White);

    // Create the scene cam node
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1");

    // Make it look towards the model
    node->yaw(Ogre::Degree(-45));

    // Create the pitch node
    node = node->createChildSceneNode("PitchNode1");
    node->attachObject(mCamera);

    // create a plane (the ground)
    mPlane = new Ogre::Plane(Ogre::Vector3::UNIT_Y, 0);
    Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            *mPlane, 500, 500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

    Ogre::Entity* entGround = mSceneMgr->createEntity("GroundEntity", "ground");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);

    entGround->setMaterialName("Examples/Rockwall");
    entGround->setCastShadows(false);


	/*** Path Synthesis Stuff starts here ***/
	mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ModelNode", Ogre::Vector3(0,0,0));      
    mEntity = mSceneMgr->createEntity("Blake", "BodyMesh.mesh");
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

            //spline->addPoint(point);
            mWalkList.push_back(point);
            //splines->update();
            lines->addPoint(point);
            lines->update();
        }

    }

    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    mCameraMan->injectMouseDown(arg, id);

    return true;
}


bool TutorialApplication::keyPressed(const OIS::KeyEvent &arg) {

	// reset program
	if (arg.key == OIS::KC_RETURN)
	{
		state = SET_USER_PATH ;

		mWalkList.clear();
		lines->clear();
		lines_path_done->clear();

		mWalkList.push_back(Ogre::Vector3(0, 1, 0));

	}
    if (arg.key == OIS::KC_SPACE) // toggle visibility of advanced frame stats
    {
        state = CALC_AVATAR_PATH;
		anim_state = AVATAR_ANIM_IN_CALC;
		
        // TODO: Aceder aqui ao grafo    
		// Get all animations and get an iterator
        AnimSSet = mEntity->getAllAnimationStates();
		assIte = new Ogre::AnimationStateIterator(AnimSSet->getAnimationStateIterator());

		// skip some animations just to speed up 
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();
		assIte->moveNext();

		mAnimationState = assIte->getNext();
        mAnimationState->setEnabled(true);
		mAnimationState->setLoop(false);

        // Clean old frame positions
        framePositionCollection.clear();

		// update what animation we are current doing
		animationStep = 1;
		currentAnimationError = 0xffffffff;

		// get the arc lenght of the user defined path
		arcLenghtLinesPath = (mWalkList[animationStep] - mWalkList[animationStep-1]).length();

		// initialize the animation that will be for the user defined path
		choosenAnimation.first = assIte->current()->second;
		choosenAnimation.second = 0xFFFFFFFF;
		
		// print on screen what we are doing
		Ogre::String str = "Current animation is ";
		str.append(mAnimationState->getAnimationName());
		mInfoLabel->setCaption(str);

		// initialize the animation Path, 
		animationPath = new std::vector< Ogre::AnimationState* >;
    }

    return BaseApplication::keyPressed(arg);

}

void TutorialApplication::createFrameListener(void) {
    BaseApplication::createFrameListener();
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent &evt) {

    if (state == CALC_AVATAR_PATH)	// Run all animations of the model
	{
        // If the current time lenght <= total time lenght 
        if(mAnimationState->getTimePosition() < mAnimationState->getLength() )
		{
            // Get current position
            Ogre::Vector3 currentPos = mEntity->getSkeleton()->getRootBone()->getPosition();
            framePositionCollection.push_back(currentPos);
            mAnimationState->addTime(evt.timeSinceLastFrame);
			//force animation update
			mEntity->_updateAnimation();
        }

		// if the current animation has ended
        if(mAnimationState->getTimePosition() == mAnimationState->getLength() )
		{ 
			currentArcLenght = 0;
			// Calculate the total arclenght
			while(framePositionCollection.size() > 1)
			{
				// get two points, but only discard the 1st
				Ogre::Vector3 v1 = framePositionCollection.front();
				framePositionCollection.pop_front();
				Ogre::Vector3 v2 = framePositionCollection.front();

				currentArcLenght += (v2 - v1).length();
			}
			char str[50] = "";
			sprintf(str, "Arc Lenght: %.3f", currentArcLenght);
			mInfoLabel2->setCaption(str);
			anim_state = AVATAR_ANIM_DONE;

			// search for the vector in user defined path with the arcLenght 
			if(mWalkList.size() > animationStep)
			{
				int vectIt = animationStep;
				
				s = mWalkList[vectIt-1];
				e = mWalkList[vectIt];
				Ogre::Vector3 v = e-s;
				Ogre::Real normaV = v.length();
				while(currentArcLenght > normaV )
				{
					vectIt++;
					if( vectIt == mWalkList.size() ) break;
					s = mWalkList[vectIt-1];
					e = mWalkList[vectIt];
					v = e-s;
					normaV += v.length();
				}

				// calculate any differences
				Ogre::Real dx, dp;
				dx = normaV - currentArcLenght;
				dp = v.length() - dx;

				v.normalise();
				m = s + dp*v;

				currentAnimationError = (m - mEntity->getSkeleton()->getRootBone()->getPosition()).length();
			}

			char str2[50];
			sprintf(str2, "Anim Error: %f", currentAnimationError);
			mInfoLabel3->setCaption(str2);
        }

		//this animation has ended, must check if there is another one to test
		if(anim_state == AVATAR_ANIM_DONE)	
		{
			if (assIte->hasMoreElements())
			{
				// check is this animation's arc lenght is a better match
				if(currentAnimationError <= choosenAnimation.second)
				{
					choosenAnimation.first = assIte->current()->second;
					choosenAnimation.second = currentAnimationError;
				}

				// get next animation 
				mAnimationState->setEnabled(false);
				mAnimationState->destroyBlendMask();
				mAnimationState = assIte->getNext();
				mAnimationState->setEnabled(true);
				mAnimationState->setLoop(false);
				framePositionCollection.clear();
				anim_state = AVATAR_ANIM_IN_CALC;
				
				Ogre::String stra = "Current animation is ";
				stra.append(mAnimationState->getAnimationName());
				mInfoLabel->setCaption(stra);
			}
			else
			{
				// update how many animations we have done
				animationStep++;

				// commit the best animation to the set
				animationPath->push_back( choosenAnimation.first );

				// move avatar to the end of animation so that we can set the node to that position
				mAnimationState = choosenAnimation.first;
				mAnimationState->setEnabled(true);
				mAnimationState->setLoop(false);
				mAnimationState->addTime(mAnimationState->getLength());
				mEntity->_updateAnimation();
				mNode->setPosition(mEntity->getSkeleton()->getRootBone()->getPosition() );

				// if user defined path has all been "animated" we must prepare to show the motion
				if( mWalkList.size() == animationStep )
				{
					state = SHOW_AVATAR_PATH;
					mAnimationState->setEnabled(false);
					mAnimationState->destroyBlendMask();

					// set the animation state to the 1st
					mAnimationState = animationPath->back();
					animationPath->pop_back();
					mAnimationState->setLoop(false);
					mAnimationState->setEnabled(true);
					mAnimationState->setTimePosition(0);

					Ogre::String strb = "Showing animation: ";
					strb.append(mAnimationState->getAnimationName());
					mInfoLabel->setCaption(strb);
					mInfoLabel2->hide();
					mInfoLabel3->hide();
					return true;
				}
			}
		}
	}

	// finally render the synthesized motion path
	if (state == SHOW_AVATAR_PATH) 
	{
		// if this animation reaches the end, start the next one
		if(mAnimationState->getTimePosition() == mAnimationState->getLength())
		{
			mAnimationState->setEnabled(false);
			mAnimationState->destroyBlendMask();
			
			// we still have animations in to run
			if(animationPath->size())
			{
				// update avatar's position
				mEntity->_updateAnimation();
				Ogre::Vector3 v = mEntity->getSkeleton()->getRootBone()->getPosition();
				mNode->setPosition(v.x, 0, v.z);

				// prepare the next animation to show
				mAnimationState = animationPath->back();
				animationPath->pop_back();
				mAnimationState->setEnabled(true);
				mAnimationState->setLoop(false);
				mAnimationState->setTimePosition(0);

				Ogre::String strc = "Showing animation: ";
				strc.append(mAnimationState->getAnimationName());
				mInfoLabel->setCaption(strc);
			}
			else
			{
				mInfoLabel->setCaption("Calculated Motion Path has ended");
			}
		}

		mAnimationState->addTime(evt.timeSinceLastFrame);
	}

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









