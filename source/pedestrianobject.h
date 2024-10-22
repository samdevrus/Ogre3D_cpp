#ifndef PEDESTRIANOBJECT_H
#define PEDESTRIANOBJECT_H
#include "QObject"
#include "bulletogreobject.h"
/*!
 * \brief Класс пешехода
 */
class PedestrianObject : public BulletOgreObject
{
    Q_OBJECT
    public:
        explicit PedestrianObject(SceneNode *node);

    signals:

    public slots:

    protected:
        void applyPhysics();
        void collided();
    private:
        /*!
         * \brief поддерживает скорость пешехода
         */
        void walk();
};

#endif // PEDESTRIANOBJECT_H
