#include "ogreapplication.h"
#include "scenebuilder.h"
#include "QDebug"
#include "QMapIterator"
#include "OgreConfigFile.h"
#include "playercontroller.h"
#include "cameranodecontroller.h"
#include "OgreFont.h"
#include "OgreTextAreaOverlayElement.h"
#include "OgreFontManager.h"
//const QString OgreApplication::WINDOW_CAPTION = "Carmageddon";

OgreApplication::OgreApplication():
    WINDOW_CAPTION("carmageddon"),
    COLLISION_DETECT_DISTANCE(0.1),
    mShutDown(false),
    mPhysicsToggle(true),
    mInitialized(false),
    mPause(false),
    DEBUG_DRAW(false),
    mTotalTime(1),
    TIME_TO_PLAY(90),
    mStarted(false),
    mIsMenuMode(false)
{
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
}

OgreApplication::~OgreApplication() {

    delete phyWorld;
    delete mSolver;
    delete mDispatcher;
    delete mCollisionConfig;
    delete mBroadphase;
    delete mRenderWinDec;
    delete mRoot;
}

void OgreApplication::removeObject() {
    BulletOgreObject* object = dynamic_cast<BulletOgreObject*>(sender());
    if (!object) {
        return;
    }
    object->removeBodies(*phyWorld);

}

void OgreApplication::start()
{
    if (!init())
        return;
    mRoot->startRendering();
}

void OgreApplication::stop() {
    mShutDown = true;
}

void OgreApplication::pause() {
    mPause = !mPause;
    Ogre::OverlayContainer *panel =
            static_cast<Ogre::OverlayContainer*>(
                OverlayManager::getSingleton().getOverlayElement("Pause")
                );
    if (mPause) {
        panel->show();
    } else {
        panel->hide();
    }
}

void OgreApplication::togglePhysics() {
    mPhysicsToggle = !mPhysicsToggle;
}

void OgreApplication::detectCollisions() {
    int numManifolds = phyWorld->getDispatcher()->getNumManifolds();
    for (int i=0;i<numManifolds;i++)
    {
        btPersistentManifold* contactManifold =  phyWorld->getDispatcher()->
                                                    getManifoldByIndexInternal(i);
        const btCollisionObject* obA =  (contactManifold->getBody0());
        const btCollisionObject* obB = (contactManifold->getBody1());

        BulletOgreObject *btObj1 = (BulletOgreObject*) obA->getUserPointer(),
                        *btObj2 = (BulletOgreObject*) obB->getUserPointer();


        if (!(btObj1 && btObj2)) {
            //collision between static geometry and object;
/*
            BulletOgreObject* btObj = NULL;
            if (btObj1) {
                btObj = btObj1;
            }
            if (btObj2) {
                btObj = btObj2;
            }
            //qDebug()<<btObj;
            if (btObj) {
                if (btObj->getName()!="PlayerObjNode") {
                    continue;
                }
                //qDebug()<<"static-collision "<<btObj->getName();
                int numContacts = contactManifold->getNumContacts();
                bool isCollided = false;
                for (int j=0;j<numContacts;j++)


                {
                    //qDebug()<<"collision "<<j;
                    btManifoldPoint& pt = contactManifold->getContactPoint(j);

                    static int wallCC = 0;
                     //   qDebug()<<"distance"<<pt.getDistance();
                          const btVector3& ptA = pt.getPositionWorldOnA();
                          const btVector3& ptB = pt.getPositionWorldOnB();
                          const btVector3& normalOnB = pt.m_normalWorldOnB;
                          float dy = ptA.y() - ptB.y(),
                                dx = ptA.x() - ptB.x(),
                                dz = ptA.z() - ptB.z();

                            if ( normalOnB.z() ==0 || normalOnB.y()==0 || normalOnB.x()==0) {
                                qDebug()<<"wall Collision!"
                                       << ptA.closestAxis()<< ptB.angle(ptA)
                                       <<ptA.w()
                                          << ptA.z()<< ptB.z()
                                          << normalOnB.closestAxis()
                                             <<normalOnB.w()
                                          <<++wallCC
                                          <<pt.getLifeTime()
                                            <<pt.getAppliedImpulse()
                                  //<<"dy collision"<<pt.getDistance()<<"\n"<<dx<<dy<<dz<<"\n"
                                  <<pt.getDistance()

                                  <<normalOnB.x()
                                  <<normalOnB.y()
                                  <<normalOnB.z();

                       }
                    /*
                          if (dz>2 || dz<-2) {
                              qDebug()<<"wall collision?"<<btObj->getName();
                              qDebug()<<dz;
                          }
*/
            continue;
        }

        int numContacts = contactManifold->getNumContacts();
        bool isCollided = false;
        for (int j=0;j<numContacts;j++)
        {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);

            if (pt.getDistance()<COLLISION_DETECT_DISTANCE)
            {
                isCollided  =true;
                break;
             //   qDebug()<<"distance"<<pt.getDistance();
         //       const btVector3& ptA = pt.getPositionWorldOnA();
         //       const btVector3& ptB = pt.getPositionWorldOnB();
         //       const btVector3& normalOnB = pt.m_normalWorldOnB;
               }
        }
        if (isCollided) {
            emit(objectsCollided(btObj1,btObj2));
       // qDebug()<<"nodes collisio"
       //      <<QString::fromStdString(node1->getName())
       //      <<QString::fromStdString( node2->getName());
        } else {
         //   qDebug()<<"no collision";
        }

    }

}

bool OgreApplication::renderFrameStartEvent(const Ogre::FrameEvent &evt) {
    if (mPause || mIsMenuMode) {
        return true;
    }
    if (mPhysicsToggle) {
        phyWorld->stepSimulation(1/100.f,7);
        phyWorld->debugDrawWorld();
        dbgdraw->setDebugMode(DEBUG_DRAW);
        dbgdraw->step();
        detectCollisions();
    } else {
        qDebug()<<"no physics!";
    }
    QMapIterator<QString,BulletOgreObject*> iter(SceneBuilder::BtOgreObjects);
    while (iter.hasNext()) {
        iter.next();
        BulletOgreObject* bto =  iter.value();

        bto->applyPhysics();
    }

    mTotalTime-=evt.timeSinceLastFrame;
    if (mTotalTime<=0) {
        pause();
        TextAreaOverlayElement *pauseText =
                static_cast<TextAreaOverlayElement*>(
                OverlayManager::getSingleton().
                    getOverlayElement("PauseText"));
        pauseText->setCaption("GAME OVER");

        mTotalTime = 0;
    }
    TextAreaOverlayElement *txtArea =
            static_cast<TextAreaOverlayElement*>(
                OverlayManager::getSingleton().getOverlayElement("TimeText")
                );

    int captionTextMin =(int)(mTotalTime/60),
            captionTextSec =  mTotalTime - captionTextMin*60 ;
    QString sec =   QString::number(captionTextSec);
    if (sec.length()==1) {
        sec="0"+sec;
    }

    txtArea->setCaption(QString::number(captionTextMin).toStdString()+":"+
                        sec.toStdString());
    return true;
}

bool OgreApplication::init()
{
    //Bullet initialisation.
    qDebug()<<"BULLET INIT";
    mBroadphase = new btAxisSweep3(btVector3(-10000,-10000,-10000),
                                   btVector3(10000,10000,10000));
    mCollisionConfig = new btDefaultCollisionConfiguration();

    mDispatcher = new btCollisionDispatcher(mCollisionConfig);

    mSolver = new btSequentialImpulseConstraintSolver();



    phyWorld = new btDiscreteDynamicsWorld(mDispatcher,
                                                    mBroadphase,
                                                    mSolver,
                                                    mCollisionConfig);
    phyWorld->setGravity(btVector3(0,-9.8,0));



    qDebug()<<"OGRE ROOT INIT";
    mRoot = new Ogre::Root(mPluginsCfg.toStdString());
    qDebug()<<"RECOURCES INIT";
    initResources();
    if (!configure()) return false;
    qDebug()<<"SCENE INIT";
    SceneBuilder sceneBuilder(mRoot->createSceneManager(Ogre::ST_GENERIC));
    sceneBuilder.init(mRenderWinDec->getWindow());
    qDebug()<<"RECOURCES LOAD";
    loadResources();
    qDebug()<<"CREATE SCENE";
    mSceneMgr = sceneBuilder.createScene(phyWorld);

    //----------------------------------------------------------
    // Debug drawing!
    //----------------------------------------------------------

    dbgdraw = new BtOgre::DebugDrawer(mSceneMgr->getRootSceneNode(),
                                      phyWorld);
    phyWorld->setDebugDrawer(dbgdraw);

    qDebug()<<"OIS/CAMERAMAN INIT";
    mInput = new InputListener();
    mInput->init(mRenderWinDec->getWindow());
    connect(mInput,SIGNAL(shutdown()),this,SLOT(stop()));
    connect(mInput,SIGNAL(pause()),this,SLOT(pause()));
    connect(&sceneBuilder,SIGNAL(setCameraNode(Ogre::SceneNode*)),mInput,
                                 SLOT(setCameraTarget(Ogre::SceneNode*)));

    qDebug()<<"FRAMELISTENER INIT";
    mRenderFrameListener = new RenderFrameListener(this);
    mRoot->addFrameListener(mRenderFrameListener);
    //PlayerController *playerContr = new PlayerController(mSceneMgr,"Player");
    //playerContr->connectTo(mInput);
    //connect(this,SIGNAL(processInput()),playerContr,SLOT(handleInput()));
    PlayerController *playerContr2 = new PlayerController(mSceneMgr,"PlayerObjNode");
    playerContr2->connectTo(mInput);
    connect(this,SIGNAL(processInput()),playerContr2,SLOT(handleInput()));

    CameraNodeController *camContrl = new CameraNodeController(mSceneMgr,"PlayerNode");
    camContrl->connectTo(mInput);
    connect(this,SIGNAL(processInput()),camContrl,SLOT(handleInput()));


    const QList<BulletOgreObject*> btObjects =  SceneBuilder::BtOgreObjects.values();
    for (int i = 0; i<btObjects.count();i++) {
        BulletOgreObject *btObject = btObjects.value(i);
        connect(this,SIGNAL(objectsCollided(BulletOgreObject*,BulletOgreObject*)),
                btObject,SLOT(onCollision(BulletOgreObject*,BulletOgreObject*)));
        connect(btObject,SIGNAL(killMe()),this,SLOT(removeObject()));
    }
    mMenuController = new MenuController(sceneBuilder.MENU_ITEMS);
    connect(mInput,SIGNAL(keyPress(OIS::KeyEvent)),mMenuController,
            SLOT(keyPress(OIS::KeyEvent)));
    connect(mInput,SIGNAL(keyRelease(OIS::KeyEvent)),mMenuController,
            SLOT(keyRelease(OIS::KeyEvent)));
    connect(this,SIGNAL(processInput()),mMenuController,SLOT(handleInput()));
    connect(mMenuController,SIGNAL(shutdown()),this,SLOT(stop()));
    connect(mMenuController,SIGNAL(start()),this,SLOT(startGame()));
    mIsMenuMode = true;
    mInitialized = true;
    return true;
}

void OgreApplication::startGame() {
    mMenuController->hide();
    mIsMenuMode =false;
    mStarted = true;
    mTotalTime = TIME_TO_PLAY;
}

bool OgreApplication::renderFrameEvent(const Ogre::FrameEvent &evt) {
     if(mRenderWinDec->getWindow().isClosed())
         return false;
     if(mShutDown)
         return false;
     mInput->capture();
     if (!mStarted) {
         mIsMenuMode = true;
         mMenuController->show();
     }
     if (mPause || mIsMenuMode) {
         mMenuController->handleInput();
         return true;
     }
     if (mInitialized) {
         processAnimation(evt.timeSinceLastEvent);
     }

     emit(processInput());
     return true;
}

void OgreApplication::processAnimation(float time) {
    for (int i = 0 ;i< SceneBuilder::Animations.count();i++) {
        AnimationState *state = SceneBuilder::Animations.value(i);
        state->addTime(time);

    }
}

void OgreApplication::windowClosedEvent(Ogre::RenderWindow *rw) {
    //Only close for window that created OIS
    //(the main window in these demos)
    if( rw == &(mRenderWinDec->getWindow()) )
    {
        delete mInput;
        mInput=NULL;
    }
}

void OgreApplication::windowResizedEvent(Ogre::RenderWindow* rw) {
}

void OgreApplication::initResources()
{
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg.toStdString());
    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}

void OgreApplication::loadResources() {

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    //Load Fonts
    Ogre::FontPtr font = Ogre::FontManager::getSingleton().getByName("StarWars");
    font->load();
}

bool OgreApplication::configure()
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg

    if(mRoot->restoreConfig() || mRoot->showConfigDialog())
    {
        mRenderWinDec = new RenderWindowListener(
                                mRoot->initialise(true,
                                WINDOW_CAPTION.toStdString())
                                          ,this);
        return true;
    }
    else
    {
        return false;
    }
}

