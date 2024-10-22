#include "scenebuilder.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreStaticGeometry.h"
#include "QDebug"
#include "OgreAlignedAllocator.h"
#include "playerobject.h"
#include "OgreFontManager.h"
#include "pedestrianobject.h"
QMap<QString,BulletOgreObject*> SceneBuilder::BtOgreObjects;

QList<AnimationState*> SceneBuilder::Animations;

SceneBuilder::SceneBuilder(Ogre::SceneManager *parSceneManager):
    MENU_ITEMS(QStringList()<<"START"<<"HELP"<<"EXIT")
{
    mSceneMgr = parSceneManager;
}

void SceneBuilder::init(Ogre::RenderWindow &rw) {
    createCamera();
    createViewport(rw);
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
}

void SceneBuilder::createCamera() {
    mCamera = mSceneMgr->createCamera("PlayerCam");
    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);
    //mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void SceneBuilder::createViewport(Ogre::RenderWindow &rw)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = rw.addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

BulletOgreObject* SceneBuilder::createBtOgreObject(SceneNode *node,
                                                   Entity *ent) {
    BulletOgreObject *btObj = new BulletOgreObject(node);

    BtOgreObjects.insert(QString::fromStdString(node->getName()),btObj);
    mPhysicWorld->addRigidBody(btObj->attachObject(ent));
    return btObj;
}

void SceneBuilder::createLight() {
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.8,0.8,0.8));
    Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_DIRECTIONAL);
    pointLight->setPosition(Ogre::Vector3(250, 450, 250));
    pointLight->setDiffuseColour(Ogre::ColourValue::White);
    pointLight->setSpecularColour(Ogre::ColourValue::White);
    mSceneMgr->setShadowTechnique(Ogre::SHADOWDETAILTYPE_INTEGRATED);
}

void SceneBuilder::createGround() {
    ///----------------------------------------------------------
    // Ground!
    //----------------------------------------------------------
    mGroundEntity = mSceneMgr->createEntity("groundEntity", "tlevel.mesh");
    Ogre::SceneNode* groundNode = mSceneMgr->getRootSceneNode()->
                                    createChildSceneNode("groundNode");
    groundNode->attachObject(mGroundEntity);

    groundNode->scale(Ogre::Vector3(1000,200,1000));
    //Create the ground shape.
    BtOgre::StaticMeshToShapeConverter converter2(mGroundEntity);
    mGroundShape = converter2.createTrimesh();
    //Create MotionState (no need for BtOgre here, you can use it if you want to though).
    btDefaultMotionState* groundState = new btDefaultMotionState(
        btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));

    //Create the Body.
    mGroundBody = new btRigidBody(0, groundState, mGroundShape, btVector3(0,0,0));

    mPhysicWorld->addRigidBody(mGroundBody);

}

void SceneBuilder::createOverlay() {
      Ogre::OverlayManager& overlayManager =
            Ogre::OverlayManager::getSingleton();
      // Create an overlay
      Ogre::Overlay* overlay = overlayManager.create( "Controls");
      mMenuOverlay = overlayManager.create( "MainMenu");

      // Create a panel
      Ogre::OverlayContainer* carPanel = createTextPanel("Car","0");
      carPanel->setPosition( 0.01, 0.9 );
      overlay->add2D( carPanel );
      Ogre::OverlayContainer* pausePanel = createTextPanel("Pause","Pause");
      pausePanel->setPosition( 0.45, 0.45 );
      pausePanel->setColour(Ogre::ColourValue(0.2,0.4,0.5,0.6));
      pausePanel->setCaption("caption");

      overlay->add2D( pausePanel );

      Ogre::OverlayContainer* timePanel = createTextPanel("Time","0");
      timePanel->setPosition( 0.9, 0.01 );
      overlay->add2D( timePanel );
      Ogre::OverlayContainer* scorePanel = createTextPanel("Score","Score:0");
      scorePanel->setPosition( 0.01, 0.01 );

      overlay->add2D( scorePanel );
      pausePanel->hide();
      overlay->show();
      createMenu();
}

Ogre::OverlayContainer * SceneBuilder::createTextPanel(QString panelName, QString text) {
    // Create a panel
    Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
    Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(
                overlayManager.createOverlayElement( "Panel", panelName.toStdString() ) );
    panel->setPosition( 0.0, 0.0 );
    panel->setDimensions( 0.1, 0.1 );

    TextAreaOverlayElement* textElement =
            createTextElement(QString(panelName+"Text"),text);
    panel->addChild(textElement);
    return panel;
}

Ogre::SceneManager *SceneBuilder::createScene(btDynamicsWorld *physicsWorld) {
    mPhysicWorld = physicsWorld;
    mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
    createLight();
    Ogre::Entity *mEntity =
            mSceneMgr->createEntity("PlayerObj", "truck2.mesh");
    mPlayerNode =  mSceneMgr->getRootSceneNode()->
                createChildSceneNode("PlayerObjNode");
    mPlayerNode->scale(Ogre::Vector3(20,20,20));
    Ogre::SceneNode *mCameraNode = mSceneMgr->
            getSceneNode("PlayerObjNode")->
            createChildSceneNode("PlayerNode");
    mCameraNode->setPosition(0,20,-75);
    mCameraNode->attachObject(mCamera);
    mCamera->lookAt(mPlayerNode->getPosition());
    BulletOgreObject *bto = new PlayerObject(mPlayerNode);
    BtOgreObjects.insert(QString::fromStdString(mPlayerNode->getName()),bto);
    mPhysicWorld->addRigidBody(bto->attachObject(mEntity));
    bto->setPosition(-900,-100,10);

    createPedestrians();
    createGround();
    createOverlay();
    return mSceneMgr;
}

void SceneBuilder::createMenu() {
    // Create a panel
    Ogre::OverlayManager& overlayManager =
            Ogre::OverlayManager::getSingleton();
    Ogre::OverlayContainer * panel =
                static_cast<Ogre::OverlayContainer*>(
                overlayManager.createOverlayElement( "Panel",
                                                     "MenuPanel" ) );
    panel->setPosition( 0.4, 0.4 );
    panel->setDimensions( 0.6, 0.6 );

    for (uint i =0; i<MENU_ITEMS.count();i++) {
        TextAreaOverlayElement *element =
                createTextElement(QString("menu"+QString::number(i)),
                                  MENU_ITEMS.value(i));
        element->setPosition(element->_getLeft()+40,element->_getTop()+i*40);

        panel->addChild(element);

    }
    TextAreaOverlayElement *element =
            createTextElement(QString("menuPointer"),
                              "x");
    panel->addChild(element);
    panel->setColour(Ogre::ColourValue(0,0,0,0.5));
    panel->show();

    mMenuOverlay->add2D(panel);
    mMenuOverlay->show();
}

TextAreaOverlayElement* SceneBuilder::createTextElement(QString name,
                                                        QString caption) {
    TextAreaOverlayElement* textElement = static_cast<TextAreaOverlayElement*>(
                Ogre::OverlayManager::getSingleton().createOverlayElement(
                    "TextArea",
                    name.toStdString()));
    textElement->setMetricsMode(Ogre::GMM_PIXELS);
    textElement->setPosition(0, 0);
    textElement->setDimensions(100, 100);
    textElement->setCharHeight(42);
    textElement->setCaption(caption.toStdString());
    textElement->setFontName("StarWars");
    textElement->setColourBottom(ColourValue::Red);
    textElement->setColourTop(ColourValue(0.5,0.5,0.1));
    return textElement;
}

void SceneBuilder::createPedestrians() {
    for (int i = 0;i<50;i++) {
        Ogre::SceneNode *pedestrianNode = mSceneMgr->getRootSceneNode()->
                createChildSceneNode("pedestrian"+
                                     QString::number(i).toStdString());
        Ogre::Entity *pedestrian = mSceneMgr->
                createEntity("Pedestrian"+ QString::number(i).toStdString(),
                             "robot.mesh");
        pedestrianNode->setPosition(-2000+rand()%10000,-200+
                                           pedestrian->getBoundingBox().getSize().y,
                                    2010-rand()%10000);
        pedestrianNode->scale(5,5,5);
        BulletOgreObject *bto3 =new PedestrianObject(pedestrianNode);
        bto3->rotate(Ogre::Vector3(0,1,0),rand()%360);

        BtOgreObjects.insert(QString::fromStdString(pedestrianNode->getName()),bto3);
        mPhysicWorld->addRigidBody(bto3->attachObject(pedestrian));
        Ogre::AnimationState* animationState = pedestrian->getAnimationState("Walk");
        animationState->setLoop(true);
        animationState->setEnabled(true);
        Animations.append(animationState);
    }

}
