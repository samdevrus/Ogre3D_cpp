#include "renderframelistener.h"

RenderFrameListener::RenderFrameListener(OgreApplication *parent)
{
    mApplication = parent;
}


bool RenderFrameListener::frameStarted(const Ogre::FrameEvent &evt) {
    return mApplication->renderFrameStartEvent(evt);
}

bool RenderFrameListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    return mApplication->renderFrameEvent(evt);
}
