/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
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
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "stdafx.h"
#include "BaseApplication.h"
#include "MotionPath.h"
#include "DynamicLines.h"
#include <OgreSimpleSpline.h>
#include <deque>

class TutorialApplication : public BaseApplication {
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

protected:
    /*Taken from Ogre Basic tutorial 2*/
    virtual void createScene(void);
    virtual void createCamera(void);
    virtual void createViewports(void);

    virtual void createFrameListener(void);
    virtual bool nextLocation(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

    // needed for user input
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

private:
    Ogre::Plane * mPlane;
    MotionPath * mPath;
    Ogre::SimpleSpline *spline;
    DynamicLines *lines;
	DynamicLines *lines_path_done;
    Ogre::SceneNode *linesNode;
	Ogre::SceneNode *linesNode_path_done;

    int MODEL; // the model identifier

    Ogre::Real mDistance; // The distance the object has left to travel
    Ogre::Vector3 mDirection; // The direction the object is moving
    Ogre::Vector3 mDestination; // The destination the object is moving towards

    Ogre::AnimationState *mAnimationState; // The current animation state of the object

    Ogre::Entity *mEntity; // The Entity we are animating
    Ogre::SceneNode *mNode; // The SceneNode that the Entity is attached to
    std::deque<Ogre::Vector3> mWalkList; // The list of points we are walking to
    Ogre::Real mWalkSpeed; // The speed at which the object is moving

    //
    // lmiranda
    //
    std::deque<Ogre::Vector3> frameRootCoordinates;
    int isIdle;

};

#endif // #ifndef __TutorialApplication_h_
