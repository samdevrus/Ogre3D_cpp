#include "renderwindowlistener.h"

RenderWindowListener::RenderWindowListener(Ogre::RenderWindow *rw,
                                             OgreApplication *parent)
{
    mWindow = rw;
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
    mApplication = parent;

}

Ogre::RenderWindow& RenderWindowListener::getWindow() {
    return *mWindow;
}

RenderWindowListener::~RenderWindowListener() {

    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);

}

void RenderWindowListener::windowResized(Ogre::RenderWindow* rw)
{
    mApplication->windowResizedEvent(rw);
    //mInput->resizeMouse(*rw);
}

//Unattach OIS before window shutdown (very important under Linux)
void RenderWindowListener::windowClosed(Ogre::RenderWindow* rw)
{
    mApplication->windowClosedEvent(rw);
}
