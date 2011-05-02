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

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    MODEL = 0;

    // Create the scene node for the model
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ModelNode");

    // add the model
    switch (MODEL) {
        case 0:
            mEntity = mSceneMgr->createEntity("Robot", "robot.mesh");
            break;
        case 1:
            mEntity = mSceneMgr->createEntity("Ninja", "ninja.mesh");
            break;
        case 2:
            mEntity = mSceneMgr->createEntity("Jaiqua", "jaiqua.mesh");
            break;
    }

    mNode->attachObject(mEntity);

    // this node if for the rest of the objects
    Ogre::SceneNode *node;

    // create the light
    Ogre::Light *light = mSceneMgr->createLight("Light1");
    light->setType(Ogre::Light::LT_POINT);
    light->setPosition(Ogre::Vector3(0, 250, 0));
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue::White);

    // Create the scene node
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1", Ogre::Vector3(-400, 200, 400));

    // Make it look towards the ninja
    node->yaw(Ogre::Degree(-45));

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

//    mPath->add(Ogre::Vector3(0, 1, 0));
//    mPath->add(Ogre::Vector3(200, 1, 0));
//    mPath->add(Ogre::Vector3(200, 1, 200));
//    mPath->add(Ogre::Vector3(200, 1, 400));
//    mPath->add(Ogre::Vector3(0, 1, 200));

    mPath->add(Ogre::Vector3(0, 1, 0));
    mPath->add(Ogre::Vector3(10, 1, 0));
    mPath->add(Ogre::Vector3(10, 1, 10));
    mPath->add(Ogre::Vector3(0, 1, 10));
    mPath->add(Ogre::Vector3(0, 1, 0));

    // add the path to the walklist
    for (int i = 0; i < mPath->size(); i++) {
        mWalkList.push_back(mPath->get(i));
    }

    // define a spline
    // http://www.ogre3d.org/docs/api/html/classOgre_1_1SimpleSpline.html#_details
    spline = new Ogre::SimpleSpline();

    //create a line
    lines = new DynamicLines(Ogre::RenderOperation::OT_LINE_STRIP);
    for (int i = 0; i < mPath->size(); i++) {
        lines->addPoint(mPath->get(i));
    }
    lines->update();

    Ogre::SceneNode *linesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("lines");
    linesNode->attachObject(lines);

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
            spline->addPoint(point);
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

void TutorialApplication::createFrameListener(void) {
    BaseApplication::createFrameListener();

    // Set default values for variables
    mWalkSpeed = 100.0f;
    mDirection = Ogre::Vector3::ZERO;

    // Set idle animation
    switch (MODEL) {
        case 0:
            mAnimationState = mEntity->getAnimationState("Idle");
            break;
        case 1:
            mAnimationState = mEntity->getAnimationState("Backflip");
            break;
        case 2:
            mAnimationState = mEntity->getAnimationState("Sneak");
            break;
    }

    mAnimationState->setLoop(true);
    mAnimationState->setEnabled(true);


}

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

    if (mDirection == Ogre::Vector3::ZERO) {
        if (nextLocation()) {
            // Set walking animation
            switch (MODEL) {
                case 0:
                    mAnimationState = mEntity->getAnimationState("Walk");
                    break;
                case 1:
                    mAnimationState = mEntity->getAnimationState("Walk");
                    break;
                case 2:
                    mAnimationState = mEntity->getAnimationState("Walk");
                    break;
            }

            mAnimationState->setLoop(true);
            mAnimationState->setEnabled(true);

        }
    } else {
        Ogre::Real move = mWalkSpeed * evt.timeSinceLastFrame;
        mDistance -= move;

        if (mDistance <= 0.0f) {
            mNode->setPosition(mDestination);
            mDirection = Ogre::Vector3::ZERO;

            // Set animation based on if the robot has another point to walk to.
            if (!nextLocation()) {
                // Set Idle animation
                switch (MODEL) {
                    case 0:
                        mAnimationState = mEntity->getAnimationState("Idle");
                        break;
                    case 1:
                        mAnimationState = mEntity->getAnimationState("Backflip");
                        break;
                    case 2:
                        mAnimationState = mEntity->getAnimationState("Sneak");

                        break;
                }
                mAnimationState->setLoop(true);
                mAnimationState->setEnabled(true);
            } else {
                // Rotation Code will go here later
                Ogre::Vector3 src = mNode->getOrientation() * Ogre::Vector3::UNIT_X;
                if ((1.0f + src.dotProduct(mDirection)) < 0.0001f) {
                    mNode->yaw(Ogre::Degree(180));
                } else {
                    Ogre::Quaternion quat = src.getRotationTo(mDirection);
                    mNode->rotate(quat);
                }
            }//else

            // lmiranda
            // get control point coordinates 
            printf("Control Point (%4.2f, %4.2f, %4.2f)\n", mNode->getPosition().x, mNode->getPosition().y, mNode->getPosition().z);

            // path synthesis
            // P			-> defined path (our mPath)
            // P'			-> actual path
            // w[i]			-> ith frame
            // s(w[i])		-> arc-length of frame i
            // P(s(w[i]))	-> point in P at arc-length of frame i
            // error function is the sum of the squared distances over all frames :

            //    for(int i = 0; i < nframes; i++)
            //    {
            //            error += pow ( P'(s(w[i])) - P(s(w[i])) , 2 );
            //
            //    }



        } else {
            mNode->translate(mDirection * move);
        } // else
    }

    mAnimationState->addTime(evt.timeSinceLastFrame);
    printf("Control Point (%4.2f, %4.2f, %4.2f)\n", mNode->getPosition().x, mNode->getPosition().y, mNode->getPosition().z);

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








