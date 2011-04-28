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
TutorialApplication::TutorialApplication(void)
{
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//-------------------------------------------------------------------------------------
void TutorialApplication::createCamera(void)
{
    // create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");
    // set its position, direction  
    mCamera->setPosition(Ogre::Vector3(0,10,500));
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    // set the near clip distance
    mCamera->setNearClipDistance(5);

    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}
//-------------------------------------------------------------------------------------
void TutorialApplication::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));    
}
//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
 
	// add the ninja
	Ogre::Entity *ent = mSceneMgr->createEntity("Ninja", "ninja.mesh");
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode");
    node->attachObject(ent);
 
    // create the light
    Ogre::Light *light = mSceneMgr->createLight("Light1");
    light->setType(Ogre::Light::LT_POINT);
    light->setPosition(Ogre::Vector3(250, 150, 250));
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue::White);
 
    // Create the scene node
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode1", Ogre::Vector3(-400, 200, 400));
 
    // Make it look towards the ninja
    node->yaw(Ogre::Degree(-45));
 
    // Create the pitch node
    node = node->createChildSceneNode("PitchNode1");
    node->attachObject(mCamera);
 
    // create the second camera node/pitch node
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode2", Ogre::Vector3(0, 200, 400));
    node = node->createChildSceneNode("PitchNode2");


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
	mPath->add(Ogre::Vector3(0,1,0));
	mPath->add(Ogre::Vector3(200,1,0));
	mPath->add(Ogre::Vector3(200,1,200));
	mPath->add(Ogre::Vector3(200,1,400));
	mPath->add(Ogre::Vector3(0, 1, 200));


	//create a line
	lines = new DynamicLines(Ogre::RenderOperation::OT_LINE_STRIP);
    for (int i=0; i<mPath->size(); i++) {
		lines->addPoint(mPath->get(i));
    }
    lines->update();
    
    Ogre::SceneNode *linesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("lines");
    linesNode->attachObject(lines);

}


bool TutorialApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	// left mouse button pressed
	if (id == OIS::MB_Left)
	{

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

		if(result.first)
		{
			// if the ray intersect the plane, we have a distance value
			// telling us how far from the ray origin the intersection occurred.
			// the last thing we need is the point of the intersection.
			// Ray provides us getPoint() function which returns a point
			// along the ray, supplying it with a distance value.

			// get the point where the intersection is
			Ogre::Vector3 point = mouseRay.getPoint(result.second);

          
			//TODO: add to the path
			printf("Mouse pressed\n");
			printf("X: %f  Y: %f  Z: %f\n", point.x, point.y, point.z);
			mPath->add(point);
			// splines->addPoint(point);
			// splines->update();
			lines->addPoint(point);
			lines->update();
		}
       
	}

	if (mTrayMgr->injectMouseDown(arg, id)) return true;
    mCameraMan->injectMouseDown(arg, id);
    
	return true;
}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
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








//~ using System;
//~ using System.Collections.Generic;
//~ using System.Linq;
//~ using System.Text;
//~ using Mogre;

//~ namespace GameEngine.GameMath
//~ {
    public static class CatmulleRome{
        public static Vector3 GetPoint(Vector3 a,Vector3 b,Vector3 ka,Vector3 kb,float u)
        {
            float u2 = u * u;
            float u3 = u2 * u;

            return ka * (-0.5f * u3 + u2 - 0.5f * u) + a * (1.5f * u3 - 2.5f * u2 + 1.0f) +
                b * (-1.5f * u3 + 2.0f * u2 + 0.5f * u) + kb * (0.5f * u3 - 0.5f * u2);
        }

        public static List<Vector3> GetPoints(Vector3 a,Vector3 b,Vector3 ka,Vector3 kb,int middlePointsCount,bool addBorderPoints)
        {
            List<Vector3> pts = new List<Vector3>(middlePointsCount + 2);
            if(addBorderPoints==true)
                pts.Add(a); // GetPoint(a, b, ka, kb, 0);
            for(int i=1;i<=middlePointsCount;i++)
            {
                float u = (float)i / (middlePointsCount+1);
                pts.Add(GetPoint(a, b, ka, kb, u));
            }
            if(addBorderPoints==true)
                pts.Add(b); // GetPoint(a, b, ka, kb, 1);
            return pts;
        }

        public static List<Vector3> GetPoints(Vector3[] basePoints,int middlePointsPerPair)
        {
            if(basePoints.Length < 2) throw new ApplicationException("Base points count must be greater than 2.");
            else if(basePoints.Length==2)
                return GetPoints(basePoints[0],basePoints[1],basePoints[0],basePoints[1],middlePointsPerPair,true);

            int ptsCount = basePoints.Length + (basePoints.Length - 1) * middlePointsPerPair;
            List<Vector3> pts = new List<Vector3>(ptsCount);

            Vector3 ka = basePoints[0];
            Vector3 kb = basePoints[0];
            for (int i = 0; i < basePoints.Length - 1; i++)
            {
                if(i!=0) ka=basePoints[i-1];

                if (i == basePoints.Length - 2)
                    kb = basePoints[i + 1];
                else
                    kb = basePoints[i + 2];
                pts.Add(basePoints[i]);
                pts.AddRange(GetPoints(basePoints[i], basePoints[i + 1], ka, kb, middlePointsPerPair, false));
            }
            pts.Add(kb);

            return pts;
        }
    }

    public static class BezierCubic
    {
        public static Vector3 GetPoint(Vector3 a, Vector3 b, Vector3 ka, Vector3 kb, float u)
        {
            float u_1 = u - 1;
            return a * -(u_1 * u_1 * u_1) + ka * (3 * (u_1 * u_1) * u) +
                kb * -(3 * u_1 * u * u) + b * (u * u * u);
        }
    }

    public static class BezierQuadratic
    {
        public static Vector3 GetPoint(Vector3 a, Vector3 b, Vector3 kp, float u)
        {
            float I_u = 1 - u;
            return a * (I_u * I_u) + kp * (2 * u * I_u) +
                b * (u * u);
        }
    }
//~ }

