#include "nodecontroller.h"
#include "OgreSceneManager.h"
#include "QDebug"

NodeController::NodeController(Ogre::SceneManager *manager, QString parNodeName) :
    QObject(0),
    mNode(0)
{
    mNodeName = parNodeName;
    mSceneMgr = manager;
    mNode = mSceneMgr->getSceneNode(mNodeName.toStdString());
    mBtOgreObject = SceneBuilder::BtOgreObjects.value(parNodeName);
    mCamera = mSceneMgr->getCamera("PlayerCam");
}

void NodeController::connectTo(InputListener *listener) {

    connect(listener,SIGNAL(keyPress(OIS::KeyEvent)),
            this,SLOT(keyPress(OIS::KeyEvent)));
    connect(listener,SIGNAL(keyRelease(OIS::KeyEvent)),
            this,SLOT(keyRelease(OIS::KeyEvent)));
    connect(listener,SIGNAL(mouseMove(OIS::MouseEvent)),
            this,SLOT(mouseMove(OIS::MouseEvent)));
    connect(listener,SIGNAL(mousePress(OIS::MouseEvent,OIS::MouseButtonID)),
            this,SLOT(mousePress(OIS::MouseEvent,OIS::MouseButtonID)));
    connect(listener,SIGNAL(mouseRelease(OIS::MouseEvent,OIS::MouseButtonID)),
            this,SLOT(mouseRelease(OIS::MouseEvent,OIS::MouseButtonID)));

}
