#ifndef RENDERFRAMELISTENER_H
#define RENDERFRAMELISTENER_H
#include "OgreFrameListener.h"
#include "ogreapplication.h"

class OgreApplication;

/*!
 * \brief Класс обработки событий построения графики
 *
 *  вызывает методы OgreApplication для обработки событий
 */
class RenderFrameListener : public Ogre::FrameListener

{
    public:
        RenderFrameListener(OgreApplication *parent);
    protected:
        bool frameStarted(const Ogre::FrameEvent &evt);
        bool frameRenderingQueued(const Ogre::FrameEvent& evt);
        OgreApplication *mApplication;


};

#endif // RENDERFRAMELISTENER_H
