#ifndef BULLETOGREOBJECT_H
#define BULLETOGREOBJECT_H
#include "OgreSceneNode.h"
#include "OgreSceneManager.h"
#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "QString"
#include "QList"
#include "QObject"
#include <BulletCollision\CollisionDispatch\btGhostObject.h>

using namespace Ogre ;

/*!
 * \brief Класс объекта, который может перемещаться сам
 *
 * Класс обеспечивает передачу данных между Bullet и Ogre.
 * Позволяет перемещать объект.
 */
class BulletOgreObject:public QObject
{
     Q_OBJECT
    public:
    /*!
         * \brief конструктор
         * \param узел, которым будет управлять обхект
         */
        BulletOgreObject(SceneNode *node);
        ~BulletOgreObject();
        /*!
         * \brief присоединение объекта к узлу и создание физической модели
         * \param модель Ogre
         * \return модель твердого тела Bullet
         */
        virtual btRigidBody* attachObject(Entity *obj);
        /*!
         * \brief установка позиции объекта
         * \param новая позиция
         */
        virtual void setPosition( const Vector3 &pos);
        /*!
         * \brief установка позиции объекта
         *
         */
        virtual void setPosition(Real x, Real y, Real z);
        /*!
         * \brief поворот объекта
         * \param вектор
         * \param угол поворота
         */
        void rotate(Vector3 vector,float grad);

        SceneNode *getSceneNode();
        /*!
         * \brief получить имя
         * \return возвращает имя узла
         */
        QString getName();

        void removeBodies(btDynamicsWorld &world);
        /*!
         * \brief применение изменений к системе
         *
         * вызывается при кажом кадре,
         * трансформация узла в соответствии с действиями
         */
        virtual void applyPhysics();
        /*!
         * \brief увеличить скорость
         */
        void accelerate();
        /*!
         * \brief уменьшить скорость(до минимальной)
         */
        void decelerate();
        /*!
         * \brief повернуть налево
         */
        void turnLeft();
        /*!
         * \brief повернуть направо
         */
        void turnRight();
        /*!
         * \brief сбросить позицию и скорость
         */
        void restore();
        /*!
         * \brief получение скорости
         * \return скорость
         */
        float getSpeed();
    signals:
        void killMe();
    public slots:
        /*!
         * \brief действие при коллизии объектов
         * \param объект 1
         * \param объект 2
         */
        virtual void onCollision(BulletOgreObject* obj1,BulletOgreObject* obj2);
        /*!
         * \brief скрыть
         */
        void hide();
        /*!
         * \brief показать
         */
        void show();
        /*!
         * \brief удалить
         */
        void remove();
    protected:
        /*!
         * \brief действие при коллизии текущего объекта с другим
         * \param другой объект
         */
        virtual void collision(BulletOgreObject* collidedObject);
        /*!
         * \brief обновление физической модели bullet
         */
        void updateMotionState();
        /*!
         * \brief количество столкновений
         */
        int mCollisionCount;
        /*!
         * \brief проверка на столкновение с препятствиями(
         * неподвижными объектами)
         */
        void checkObstacle();
        SceneNode *mNode;
        /*!
         * \brief активность объекта за последнее время
         */
        float mActive;

        btScalar mMass,mFriction;
        QList<Vector3> mLastPositions;
        Entity * mEntity;
        QList<btRigidBody*> mBodies;
        QList<btCollisionShape*> mShapes;
        /*!
         * \brief Реакция объекта на столкновение
         */
        virtual void collided();
        float mSpeed;
        bool mResetOrientation,mKeepOrientationY;

        const float MAX_SPEED, FRICTION_FORCE,ACCELERATION_STEP,BREAKING_FORCE,MIN_SPEED;

};


#endif // BULLETOGREOBJECT_H
