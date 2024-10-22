#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <QObject>
#include "OIS.h"
#include "Ogre.h"
#include "QStringList"
class MenuController : public QObject
{
    Q_OBJECT
    public:
        explicit MenuController(QStringList menuItems);

    signals:
        void shutdown();
        void start();
        void help();
    public slots:
        void show();
        void hide();
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
        virtual void handleInput() {
            for (int i = 0; i<pressedKeys.count();i++) {
                keyPressed(pressedKeys.at(i).key);
            }
        };
    private:
        virtual void keyPressed(OIS::KeyCode code);
        int mMenuIndex;
        QStringList mMenuItems;
        bool enabled;
        Ogre::OverlayElement* mMenuPointer;
        QList<OIS::KeyEvent> pressedKeys;
};

#endif // MENUCONTROLLER_H
