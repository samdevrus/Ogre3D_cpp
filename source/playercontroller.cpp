#include "playercontroller.h"
#include "QDebug"
#include "OgreSceneManager.h"
#include "OgreCamera.h"
using namespace OIS;

PlayerController::PlayerController(Ogre::SceneManager *manager,
                                   QString parNodeName) :
    NodeController(manager,parNodeName)
{
    //mSpeed = 0;
    mCar =dynamic_cast<PlayerObject*>(mBtOgreObject);
}


void PlayerController::keyPressed(const OIS::KeyCode code) {


    Ogre::Vector3 position = mNode->getPosition();
/*
    qDebug()<<"NODE_POITION : "<<
              mNode->getOrientation().getRoll().valueAngleUnits()
              <<mNode->getOrientation().getYaw().valueAngleUnits()
             <<mNode->getOrientation().getPitch().valueAngleUnits()
             ;
*/
    float rad = mNode->getOrientation().getYaw().valueRadians();
    switch(code) {
        //move left
        case KC_A: {
        mCar->turnLeft();
            break;
    }
        //right
    case KC_D: {
        mCar->turnRight();

        break;
    }
        //forward / start
    case KC_W: {
        mCar->accelerate();
        break;
    }
        //down /stop
    case KC_S: {
        mCar->decelerate();
        break;
    }
       // case KC_Q:position.y+=1;break;
       // case KC_E:position.y-=1;break;
        case KC_R:mCar->restore();
    }


 //   qDebug()<<"press"<<position.x;
    //if (mBtOgreObject) {
    //    mBtOgreObject->setPosition(position);
    //} else {
      //  qDebug()<<"ignore bt";

        //mNode->setPosition(position);
    //}
    //qDebug()<<"speed"<<speed<<step;
}

void PlayerController::mouseMove(const MouseEvent arg) {
    Ogre::Camera *cam = mSceneMgr->getCamera("PlayerCam");
    cam->yaw(Ogre::Degree(-arg.state.X.rel * 0.15f));
    cam->pitch(Ogre::Degree(-arg.state.Y.rel * 0.15f));
}

