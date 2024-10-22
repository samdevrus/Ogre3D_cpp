#include "pedestrianobject.h"
#include "QDebug"
PedestrianObject::PedestrianObject(SceneNode *node) :
    BulletOgreObject(node)
{
    mMass = 50;
    mKeepOrientationY = true;
}

void debugVector(Ogre::Vector3 position) {
    qDebug()<<
              position.x<<position.y<<position.z;
}

void PedestrianObject::applyPhysics() {
    walk();
    BulletOgreObject::applyPhysics();
}

void PedestrianObject::walk() {
    if (mSpeed<2) {
        accelerate();
    }
    //mSpeed = -2;
    //rotate(Ogre::Vector3(0,1,0), 0.1);
}

void PedestrianObject::collided() {
    qDebug()<<"pedestrian collided";
    mSpeed = -2;
   // restore();
   // rotate(Ogre::Vector3(0,1,0), 90);
    //rotate(Ogre::Vector3(0,1,0), 90);
   // restore();
    mNode->pitch(Radian(0));
    mNode->roll(Radian(0));
    mNode->yaw(Radian(0));

    mNode->resetOrientation();
    rotate(Ogre::Vector3(0,1,0),rand()%360);
    //mSpeed =5;
    //mActive = 4;
}
