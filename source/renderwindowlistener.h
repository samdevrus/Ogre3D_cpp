#ifndef RENDERWINDOWLISTENER_H
#define RENDERWINDOWLISTENER_H
#include "OgreWindowEventUtilities.h"
#include "inputlistener.h"
#include "QObject"
#include "ogreapplication.h"

class OgreApplication;
/*!
 * \brief Класс обработки события окна
 *
 *  вызывает методы OgreApplication для обработки событий
 */
class RenderWindowListener:public Ogre::WindowEventListener
{
    public:
        RenderWindowListener(Ogre::RenderWindow *rw,OgreApplication *parent);
        ~RenderWindowListener();
        //void setInputListener(InputListener *listener);
        Ogre::RenderWindow& getWindow();

    protected:
        void windowClosed(Ogre::RenderWindow* rw);
        void windowResized(Ogre::RenderWindow* rw);
        Ogre::RenderWindow* mWindow;
        OgreApplication *mApplication;

};

#endif // RENDERWINDOWLISTENER_H
