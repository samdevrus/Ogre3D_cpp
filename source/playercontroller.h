#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H
#include "nodecontroller.h"
#include "playerobject.h"
/*!
 * \brief Контроллер игрока
 */
class PlayerController : public NodeController
{
    Q_OBJECT
    public:
        explicit PlayerController(Ogre::SceneManager *manager,
                                  QString parNodeName);

    signals:

    public slots:
       // void keyPress(const OIS::KeyEvent arg);
       // void keyRelease(const OIS::KeyEvent arg);
    protected:
        void keyPressed(OIS::KeyCode code);
        void mouseMove(const OIS::MouseEvent arg);
        PlayerObject *mCar;

};

#endif // PLAYERCONTROLLER_H
