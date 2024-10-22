#include "cameranodecontroller.h"
#include "QDebug"

CameraNodeController::CameraNodeController(SceneManager *manager, QString parNodeName) :
    NodeController(manager,parNodeName),
    CAMERA_Y_MIN(5),
    CAMERA_MIN(-100,5,-100),
    CAMERA_MAX(200,50,200)
{
    mCamera = manager->getCamera("PlayerCam");
    mTarget = manager->getSceneNode("PlayerObjNode");
}

void CameraNodeController::mouseMove(const OIS::MouseEvent arg) {

    Ogre::Vector3 pos = mNode->getPosition();
   // qDebug()<<arg.state.X.rel<<arg.state.Z.rel;
    pos.y+=arg.state.Y.rel;
    if (pos.y<CAMERA_MIN.y) {
        pos.y=CAMERA_MIN.y;
    }
    if (pos.y>CAMERA_MAX.y) {
        pos.y=CAMERA_MAX.y;
    }
    pos.x+=arg.state.X.rel;
    if (pos.x<CAMERA_MIN.x) {
        pos.x=CAMERA_MIN.x;
    }
    if (pos.x>CAMERA_MAX.x) {
        pos.x=CAMERA_MAX.x;
    }

    float dWheel = arg.state.Z.rel;
    if (dWheel!=0) {
        dWheel = dWheel/10;
        pos.z+=dWheel;
    }
    if (pos.z<CAMERA_MIN.z) {
        pos.z=CAMERA_MIN.z;
    }
    if (pos.z>CAMERA_MAX.z) {
        pos.z=CAMERA_MAX.z;
    }

    mNode->setPosition(pos);
    if (mTarget) {
        mCamera->lookAt(mTarget->getPosition());
    }
}
