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

#include "Lines/DynamicLines.h"
#include <OgreSimpleSpline.h>
#include <deque>


#define SET_USER_PATH   	0
#define CALC_AVATAR_PATH	1
#define SHOW_AVATAR_PATH	2
#define AVATAR_ANIM_IN_CALC	3
#define AVATAR_ANIM_DONE	4
#define AVATAR_PATH_DONE	5

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

protected:
    virtual void createScene(void);
	virtual void createCamera(void);
    virtual void createViewports(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool keyPressed( const OIS::KeyEvent &arg );

	void loadAnimationNames() ;
	
//	void _loadSingleAnimationScene( CDotScene &scene, std::string scene_name,std::string animation_name);
	void _loadFullScene( /*CDotScene &scene*/);
	
	void _loadEntityNames();


private:
    Ogre::Plane * mPlane;
	std::vector<Ogre::Vector3> * mPath;

    Ogre::SimpleSpline *spline;
    DynamicLines *lines;
    DynamicLines *lines_path_done;
    Ogre::SceneNode *linesNode;
    Ogre::SceneNode *linesNode_path_done;

	Ogre::Real arcLenghtLinesPath; // the arc lenght of the user defined path
	int animationStep;	//how many animations we have done so far 

    Ogre::Real mDistance; // The distance the object has left to travel
    Ogre::Vector3 mDirection; // The direction the object is moving
    Ogre::Vector3 mDestination; // The destination the object is moving towards

    Ogre::AnimationState *mAnimationState; /* The current animation state of the object */
	Ogre::AnimationStateSet *AnimSSet;
	Ogre::AnimationStateIterator *assIte;
	Ogre::Real totalLenght ;
	Ogre::Real currentArcLenght;
	Ogre::Real currentAnimationError;

    Ogre::Entity *mEntity; // The Entity we are animating
    Ogre::SceneNode *mNode; // The SceneNode that the Entity is attached to
    std::deque<Ogre::Vector3> mWalkList; // The list of points we are walking to
    Ogre::Real mWalkSpeed; // The speed at which the object is moving

	// the animation we will use <animation, error>
	std::pair<Ogre::AnimationState*, Ogre::Real> choosenAnimation;	

	// the set of animations that wil be used for the entire path
	//std::vector< Ogre::AnimationState* > * animationPath;
	std::vector< Ogre::String > * animationPath;
	// a copy
	std::vector< Ogre::AnimationState* > * animationPath_copy;

    int state;
	int anim_state;
    std::deque<Ogre::Vector3> framePositionCollection;	// positions of the root bone of the model at each frame


	Ogre::Vector3 s;
	Ogre::Vector3 e;
	Ogre::Vector3 m;


};

#endif // #ifndef __TutorialApplication_h_
