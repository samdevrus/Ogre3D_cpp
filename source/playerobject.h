#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H
#include "bulletogreobject.h"
#include "QObject"

/*!
 * \brief Класс объекта игрока
 */
class PlayerObject : public BulletOgreObject
{
    Q_OBJECT
    public:
        explicit PlayerObject(SceneNode *node);
    protected:
        void applyPhysics();
        void collision(BulletOgreObject *collidedObject);
    private:
        /*!
         * \brief эмуляция физики машины
         *
         * эмулирует силу трения
         */
        void carPhysics();
        int mScore;

};

#endif // PLAYEROBJECT_H
