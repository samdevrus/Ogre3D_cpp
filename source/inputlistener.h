#ifndef INPUTLISTENER_H
#define INPUTLISTENER_H

#include <QObject>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include "OgreRenderWindow.h"
#include "SdkCameraMan.h"
/*!
 * \brief Слушатель событий OIS
 *
 * Слушает события ввода и передает сигналы
 */
class InputListener :   public QObject,
                        public OIS::KeyListener,
                        public OIS::MouseListener
{
    Q_OBJECT
    public:
        explicit InputListener();
        /*!
         * \brief  инициализация
         * \param окно
         */
        void init(Ogre::RenderWindow &rw);
        ~InputListener();
    signals:
        /*!
         * \brief завершить работу
         */
        void shutdown();
        /*!
         * \brief сделать паузу
         */
        void pause();
        /*!
         * \brief переключить физику
         */
        void togglePhysics();

        /*!
         * \brief нажатие на клавижу
         * \param arg
         */
        void keyPress(const OIS::KeyEvent arg);
        /*!
         * \brief отжатие клавижы
         * \param arg
         */
        void keyRelease( const OIS::KeyEvent arg );
        /*!
         * \brief движение мышкой
         * \param arg
         */
        void mouseMove( const OIS::MouseEvent arg );
        void mousePress( const OIS::MouseEvent arg, OIS::MouseButtonID id );
        void mouseRelease( const OIS::MouseEvent arg, OIS::MouseButtonID id );
    public slots:
        /*!
         * \brief считать данные с устройств ввода
         */
        void capture();
        void resizeMouse(Ogre::RenderWindow &rw);
    protected:
        // OIS::KeyListener
        bool keyPressed( const OIS::KeyEvent &arg );
        bool keyReleased( const OIS::KeyEvent &arg );
        // OIS::MouseListener
        bool mouseMoved( const OIS::MouseEvent &arg );
        bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
        bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
        //OIS Input devices
        OIS::InputManager* mInputManager;
        OIS::Mouse*    mMouse;
        OIS::Keyboard* mKeyboard;
};

#endif // INPUTLISTENER_H
