#ifndef OGREAPPLICATION_H
#define OGREAPPLICATION_H
#include "OgreRoot.h"
#include "QString"
#include "inputlistener.h"
#include "renderwindowlistener.h"
#include "renderframelistener.h"
#include "QObject"
#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "bulletogreobject.h"
#include "BtOgreExtras.h"
#include "menucontroller.h"
class RenderFrameListener;
class RenderWindowListener;

/*!
 * \brief  Приложение Ogre запускающее 3D отрисовку
 */
class OgreApplication: public QObject

{
    Q_OBJECT
    public:
        OgreApplication();
        ~OgreApplication();
        /*!
         * \brief запуск программы
         */
        void start();
        /*!
         * \brief действие при обработке кадра
         * \param evt
         * \return
         */
        bool renderFrameEvent(const Ogre::FrameEvent &evt);
        /*!
         * \brief действие при начале обработки кадра
         * \param evt
         * \return
         */
        bool renderFrameStartEvent(const Ogre::FrameEvent &evt);
        void windowResizedEvent(Ogre::RenderWindow* rw);
        void windowClosedEvent(Ogre::RenderWindow* rw);
    signals:
        /*!
         * \brief считать ввод
         */
        void processInput();
        /*!
         * \brief Столкновение пожвижных объектоа
         * \param объект 1
         * \param объект 2
         */
        void objectsCollided(BulletOgreObject *object1,BulletOgreObject *object2);
    public slots:
        /*!
         * \brief остановить работу
         */
        void stop();
        void startGame();
        void removeObject();
        void togglePhysics();
        /*!
         * \brief приостановить работу
         */
        void pause();
    protected:
        /*!
         * \brief инициализация
         * \return true если успешно
         */
        bool init();
        /*!
         * \brief инициализация ресурсов
         */
        void initResources();
        void loadResources();
        /*!
         * \brief определение коллизий объектов
         */
        void detectCollisions();
        /*!
         * \brief анимациая объктов
         * \param время анимации
         */
        void processAnimation(float time);
        /*!
         * \brief конфигурирование настроек графики
         * \return true если успешно
         */
        bool configure();

        float mTotalTime;
        Ogre::Root *mRoot;
        RenderWindowListener *mRenderWinDec;
        RenderFrameListener *mRenderFrameListener;
        InputListener *mInput;
        Ogre::SceneManager *mSceneMgr;
        QString mResourcesCfg,mPluginsCfg;
        bool mShutDown,mPhysicsToggle,mInitialized,mPause,
                mIsMenuMode,
                mStarted;
        MenuController *mMenuController;
        const QString WINDOW_CAPTION;
        const float COLLISION_DETECT_DISTANCE;
        const int TIME_TO_PLAY;
        const bool DEBUG_DRAW;
        btDynamicsWorld *phyWorld;
        BtOgre::DebugDrawer *dbgdraw;
        btAxisSweep3 *mBroadphase;
        btDefaultCollisionConfiguration *mCollisionConfig;
        btCollisionDispatcher *mDispatcher;
        btSequentialImpulseConstraintSolver *mSolver;

};

#endif // OGREAPPLICATION_H
