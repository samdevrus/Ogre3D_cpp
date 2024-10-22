#include "playerobject.h"
#include "QDebug"
#include "OgreTextAreaOverlayElement.h"
#include "OgreOverlayManager.h"
#include "pedestrianobject.h"



PlayerObject::PlayerObject(SceneNode *node) :
    BulletOgreObject(node),
  mScore(0)
{
    mMass = 1000;
    mFriction =80;
    //mSpeed = 0;
   // mKeepOrientationY=true;
}

void PlayerObject::collision(BulletOgreObject *collidedObject) {
    //qDebug()<<mCollisionCount;
    PedestrianObject *pedestrian = dynamic_cast<PedestrianObject*>(collidedObject);
    qDebug()<<pedestrian;
    if (!pedestrian) {
        return;
    }
    collidedObject->remove();
    TextAreaOverlayElement *txtArea =
            static_cast<TextAreaOverlayElement*>(
                OverlayManager::getSingleton().getOverlayElement("ScoreText")
                );
    //qDebug<<
    mScore+=abs(mSpeed/MAX_SPEED*10);
    //= abs(mSpeed*3);
    txtArea->setCaption("Score: "+QString::number(mScore).toStdString());
}

void PlayerObject::applyPhysics() {

    carPhysics();
    BulletOgreObject::applyPhysics();
    TextAreaOverlayElement *txtArea =
            static_cast<TextAreaOverlayElement*>(
                OverlayManager::getSingleton().getOverlayElement("CarText")
                );

    int captionSpeed = abs(mSpeed*3);
    txtArea->setCaption(QString::number(captionSpeed).toStdString()+" km/h");
   // qDebug()<<"speed"<<mSpeed;

}

void PlayerObject::carPhysics() {
    if (mSpeed>0) {
        mSpeed -=FRICTION_FORCE;
        if (mSpeed<0) {
            mSpeed = 0;
        }
    }
    if (mSpeed<0) {
        mSpeed +=FRICTION_FORCE;
        if (mSpeed>0) {
            mSpeed = 0;
        }
    }
}
