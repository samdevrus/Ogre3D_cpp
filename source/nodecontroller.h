#ifndef NODECONTROLLER_H
#define NODECONTROLLER_H

#include <QObject>
#include "OIS.h"
#include "inputlistener.h"
#include "OgreSceneNode.h"
#include "QList"
#include "QDebug"
#include "scenebuilder.h"

/*!
 * brief Контроллер узла Ogre
 *
 * Позволяет для заданного узла обрабатывать нажатия клавиш
 */
class NodeController : public QObject
{
    Q_OBJECT
    public:
        explicit NodeController(Ogre::SceneManager *manager,
                                QString parNodeName);
        /*!
         * \brief соединить сигналы со слотами
         * \param класс издающий сигналы
         */
        void connectTo(InputListener *listener);
    public slots:
        virtual void keyPress(const OIS::KeyEvent arg) {
            pressedKeys.append(arg);
        }
        virtual void keyRelease( const OIS::KeyEvent arg ) {

            for (int i=0;i<pressedKeys.count();i++) {
                const OIS::KeyEvent &ev = pressedKeys.at(i);
                if (arg.device==ev.device &&
                        arg.key==ev.key
                        ) {
                    pressedKeys.removeAt(i);
                }
            }

        }
        virtual void mouseMove( const OIS::MouseEvent arg ) {
            qDebug()<<"Mouse: buttons:"<<arg.state.buttons
                    <<"; height:"<<arg.state.height
                    <<"; width:"<<arg.state.width
                    <<"; x:"<<arg.state.X.rel
                    <<"; y:"<<arg.state.Y.rel
                    <<"; z:"<<arg.state.Z.rel;
        }
        virtual void mousePress( const OIS::MouseEvent arg,
                                 OIS::MouseButtonID id ) {}
        virtual void mouseRelease( const OIS::MouseEvent arg,
                                   OIS::MouseButtonID id ) {}
        virtual void handleInput() {
            for (int i = 0; i<pressedKeys.count();i++) {
                keyPressed(pressedKeys.at(i).key);
            }
        };
    protected:
        virtual void keyPressed(OIS::KeyCode code){}
        QString mNodeName;
        QList<OIS::KeyEvent> pressedKeys;
        Ogre::SceneNode *mNode;
        Ogre::SceneManager *mSceneMgr;
        BulletOgreObject *mBtOgreObject;
        Ogre::Camera *mCamera;
};

#endif // NODECONTROLLER_H
