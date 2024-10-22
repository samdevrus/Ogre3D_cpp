#include "inputlistener.h"
#include "OgreLogManager.h"
#include "application.h"
InputListener::InputListener() :
    QObject(0)
{

}

void InputListener::init(Ogre::RenderWindow &rw) {
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    rw.getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    mInputManager = OIS::InputManager::createInputSystem( pl );
    mKeyboard = static_cast<OIS::Keyboard*>(
                mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(
                mInputManager->createInputObject( OIS::OISMouse, true ));
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
    resizeMouse(rw);
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initialized ***");
}

void InputListener::resizeMouse(Ogre::RenderWindow &rw) {
    unsigned int width, height, depth;
    int left, top;
    rw.getMetrics(width, height, depth, left, top);
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

InputListener::~InputListener() {
    if (mInputManager) {
        mInputManager->destroyInputObject( mMouse );
        mInputManager->destroyInputObject( mKeyboard );

        OIS::InputManager::destroyInputSystem(mInputManager);
        mInputManager = 0;
    }
}

void InputListener::capture() {
    mKeyboard->capture();
    mMouse->capture();
}

bool InputListener::keyPressed( const OIS::KeyEvent &arg )
{
     // don't process any more keys if dialog is up
   // if (mTrayMgr->isDialogVisible()) return true;
    if (arg.key==OIS::KC_ESCAPE) {
        Ogre::LogManager::getSingletonPtr()->logMessage("ESCAPE");
        emit(shutdown());
        return true;
    }
    if (arg.key==OIS::KC_P) {
        emit(pause());
    }
    emit(keyPress(OIS::KeyEvent(arg)));
    return true;
}

bool InputListener::keyReleased( const OIS::KeyEvent &arg )
{
    emit(keyRelease(arg));
    return true;
}

bool InputListener::mouseMoved( const OIS::MouseEvent &arg )
{
    //if (mTrayMgr->injectMouseMove(arg)) return true;

    emit(mouseMove(arg));
    return true;
}

bool InputListener::mousePressed( const OIS::MouseEvent &arg,
                                  OIS::MouseButtonID id )
{
  //  if (mTrayMgr->injectMouseDown(arg, id)) return true;
   //  emit(mousePress(arg,id));
    return true;
}

bool InputListener::mouseReleased( const OIS::MouseEvent &arg,
                                   OIS::MouseButtonID id )
{
  //  if (mTrayMgr->injectMouseUp(arg, id)) return true;

   // emit(mouseReleased(arg,id));
    return true;
}
