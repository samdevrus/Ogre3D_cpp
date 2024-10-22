#ifndef CAMERANODECONTROLLER_H
#define CAMERANODECONTROLLER_H
#include "QObject"
#include "nodecontroller.h"

/*!
 * \brief Управление камерой
 */
class CameraNodeController : public NodeController
{
    Q_OBJECT
    public:
        explicit CameraNodeController(Ogre::SceneManager *manager,
                                      QString parNodeName);
    protected:
        void mouseMove(const OIS::MouseEvent arg);
        const float CAMERA_Y_MIN;
        Ogre::Vector3 CAMERA_MIN,CAMERA_MAX;
        Ogre::Camera *mCamera;
        Ogre::SceneNode *mTarget;
};

#endif // CAMERANODECONTROLLER_H
