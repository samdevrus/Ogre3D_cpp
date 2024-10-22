#include "bulletogreobject.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"
#include "QDebug"
#include "BtOgMotionState.h"



BulletOgreObject::BulletOgreObject(SceneNode *node):
    mCollisionCount(0),
    mMass(1),
    mFriction(1),
    MAX_SPEED(30),MIN_SPEED(-5),
    FRICTION_FORCE(0.005),
    ACCELERATION_STEP(0.02),
    BREAKING_FORCE(0.04),
    mSpeed(0),mActive(0),
    mResetOrientation(false),
    mKeepOrientationY(false)
{
    mNode = node;
}

void BulletOgreObject::checkObstacle()  {
  // qDebug()<<getName();
    if (mLastPositions.count()==10) {
        float distance;
       // qDebug()<<mActive;
        for (int i=0;i<mLastPositions.count()-1;i++) {
            for (int j=i+1;j<mLastPositions.count();j++) {
                float dist = mLastPositions.value(i).distance(mLastPositions.value(j));
                if (i==0 && j==1) {
                    distance = dist;
                }
                if (dist<distance) {
                    distance = dist;
                }

            }
        }
                //mLastPositions.value(0).distance(mLastPositions.value(1)),

/*
        if (getName() =="PlayerObjNode") {
            debugVector(mLastPositions.value(0));
            debugVector(mLastPositions.value(1));
            debugVector(mLastPositions.value(2));
            qDebug()<<distance;
        }
        */
        if (distance<1 && mActive>0) {
            mActive-=0.1;
        }

        if (distance>=1 && mActive<4) {
            mActive+=0.1;
        }
    }
   // if (getName() =="PlayerObjNode")
   // qDebug()<<mActive;
    if (mActive<=2) {
        if (mSpeed>2) {
        collided();
        }
    }
}

void BulletOgreObject::rotate(Vector3 vector, float grad) {
    Ogre::Radian rad(grad* 3.14/180);
    mNode->rotate(vector,rad);
    updateMotionState();
}

btRigidBody *BulletOgreObject::attachObject(Entity *obj) {
    mNode->attachObject(obj);
    BtOgre::StaticMeshToShapeConverter converter(obj);
    //Old way
    btCollisionShape *shape = converter.createBox();
    //New way
/*
    btCompoundShape* compound = new btCompoundShape();
    btTransform localTrans;
    localTrans.setIdentity();
    btCollisionShape* chassisShape =  converter.createConvex();
    compound->addChildShape(localTrans,chassisShape);
    btCollisionShape *shape = compound;
    */
    btVector3 inertia;
    shape->calculateLocalInertia(mMass, inertia);




    //Old Way
    //BtOgre::RigidBodyState *state =
    //        new BtOgre::RigidBodyState(mNode);
    //new btVehicleRaycaster
    ///btRigidBody *body = new btRigidBody(mass, state, shape, inertia);
    //new btVehicleRaycaster

    btTransform trans;
    trans.setOrigin(BtOgre::Convert::toBullet(mNode->getPosition()));
    trans.setRotation(BtOgre::Convert::toBullet(mNode->getOrientation()));
    btMotionState* state = new btDefaultMotionState(trans);

    btRigidBody::btRigidBodyConstructionInfo CI(mMass,
                                                state,
                                                shape,
                                                inertia);
    btRigidBody *mRigidBody = new btRigidBody(CI);
    mRigidBody->setFriction(mFriction);
    mRigidBody->setUserPointer(this);
    //qDebug()<<"set pointer to  node = "<<mNode;
    mRigidBody->setActivationState(DISABLE_DEACTIVATION);
    mRigidBody->setCollisionFlags(mRigidBody->getCollisionFlags()
                                  | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

    //mRigidBody->setDamping(0.05f, 0.4f);
    mShapes.append(shape);
    mBodies.append(mRigidBody);
    return mRigidBody;
}

SceneNode* BulletOgreObject::getSceneNode() {
    return mNode;
}

QString BulletOgreObject::getName() {
    return QString::fromStdString(mNode->getName());
}

void BulletOgreObject::onCollision(BulletOgreObject *obj1, BulletOgreObject *obj2) {
    BulletOgreObject *collidedObject = NULL;
    static int collisionCount = 0;
    if (this==obj1) {
        collidedObject = obj2;
    }
    if (this==obj2) {
        collidedObject = obj1;
    }
    if (!collidedObject) {
        return;
    }
    collisionCount++;
    mCollisionCount++;
    collision(collidedObject);
  // qDebug()<<"I ("<<getName()<<") collided with "<<collidedObject->getName()
  //         <<" num "<<collisionCount;



}

void BulletOgreObject::remove() {
    hide();
    emit(killMe());
}

void BulletOgreObject::removeBodies(btDynamicsWorld &world) {
    for (int i =0; i<mBodies.count();i++) {
        world.removeRigidBody(mBodies.value(i));
    }
    //hide();
}

void BulletOgreObject::hide() {
    mNode->setVisible(false);
}


void BulletOgreObject::show() {
    mNode->setVisible(true);
}

void BulletOgreObject::collision(BulletOgreObject *collidedObject) {

}

BulletOgreObject::~BulletOgreObject() {
    for (int i=0; i<mShapes.count();i++) {
        delete mShapes.value(i);
    }
    for (int i=0;i<mBodies.count();i++) {
        delete mBodies.value(i);
    }
}

void BulletOgreObject::setPosition(const Vector3 &pos) {
    mNode->setPosition(pos);

   // standUp();
    updateMotionState();
}

void BulletOgreObject::setPosition(Real x, Real y, Real z) {
    mNode->setPosition(x,y,z);
//    standUp();
    updateMotionState();
}

void BulletOgreObject::updateMotionState() {
    static uint call=0;
    call++;

    for (int i=0;i<mBodies.count();i++) {
        btRigidBody *body = mBodies.value(i);
        btMotionState *state =new BtOgre::RigidBodyState(mNode);
        body->activate();
        body->setMotionState(state);
       // body->applyDamping(1);
        //qDebug()<<body->isActive();

        body->applyGravity();
    }
}

void BulletOgreObject::applyPhysics() {

    for (int i=0;i<mBodies.count();i++) {
        btRigidBody *body = mBodies.value(i);
      // body->applyGravity();
       btTransform trans = body->getWorldTransform();
       mNode->setPosition(BtOgre::Convert::toOgre(trans.getOrigin()));
       mNode->setOrientation(BtOgre::Convert::toOgre(trans.getRotation()));
    }

    checkObstacle();
    Ogre::Vector3 position = mNode->getPosition();

  //  debugVector(position);
    if (mResetOrientation) {
       mNode->resetOrientation();
       mResetOrientation = false;
    }
    if (mKeepOrientationY) {
        Quaternion rot =  mNode->getOrientation();
      //  qDebug()<<rot.x<<rot.y<<rot.z<<rot.w;
      //  rot.z = 0.1;
        //rot.w = 0;
      //  rot.x = 0.1;
       //mNode->resetOrientation();


        //mNode->setOrientation(rot);

        //mNode->yaw(Radian(-rot.getYaw().valueRadians()));
        mNode->pitch(Radian(-rot.getPitch().valueRadians()/2));
        mNode->roll(Radian(-rot.getRoll().valueRadians()/2));

       // mKeepOrientationY = false;
    }
    //INIT TEST

    if (position == Ogre::Vector3::ZERO && mSpeed ==0) {
        qDebug()<<"not init!";
        return;
    }
    if (getName()=="pedestrinan")
        qDebug()<<mSpeed<<"set pos";
    float rad = mNode->getOrientation().getYaw().valueRadians();
    position.x+=mSpeed* sin(rad);
    position.z+=mSpeed* cos(rad);


    setPosition(position);




    mLastPositions.append(mNode->getPosition());
    if (mLastPositions.count()>10) {
        mLastPositions.removeFirst();
    }
}

void BulletOgreObject::collided() {
   // if (mSpeed>5) {
   // qDebug()<<"collided";
        mSpeed = 0;
   // }
}

void BulletOgreObject::accelerate() {
    if (mSpeed>MAX_SPEED) {
        return;

    }
    //qDebug()<<"accelerate";
    mSpeed +=ACCELERATION_STEP;
}

float BulletOgreObject::getSpeed() {
    return mSpeed;
}

void BulletOgreObject::turnLeft() {
    if (!mSpeed) {
        return;
    }
    rotate(Ogre::Vector3(0,1,0),0.5);
}


void BulletOgreObject::turnRight() {
    if (!mSpeed) {
        return;
    }
    rotate(Ogre::Vector3(0,1,0),-0.5);
}
//mBtOgreObject->rotate(Ogre::Vector3(0,1,0),-1);

void BulletOgreObject::decelerate() {
    if (mSpeed<=MIN_SPEED) {
        return;

    }
    mSpeed -=BREAKING_FORCE;
    //qDebug()<<"break,"<<mSpeed;
}

void BulletOgreObject::restore() {
    //qDebug()<<"restore ";
    mSpeed = 0;
    mResetOrientation  =true;
}

