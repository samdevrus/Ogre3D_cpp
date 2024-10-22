#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "BtOgrePG.h"
#include "BtOgreGP.h"
#include "BtOgreExtras.h"
#include "bulletogreobject.h"
#include "QMap"
#include "QObject"
#include "QStringList"
#include "OgreTextAreaOverlayElement.h"
/*!
 * \brief Построитель сцены
 */
class SceneBuilder: public QObject
{
    Q_OBJECT
    public:
        SceneBuilder(Ogre::SceneManager *parSceneManager);
        /*!
         * \brief инициализация
         * \param окно в котором отображается графика
         */
        void init(Ogre::RenderWindow &rw);
        /*!
         * \brief создание сцены
         * \param физический мир bullet
         * \return сцена Ogre
         */
        Ogre::SceneManager* createScene(btDynamicsWorld *physicsWorld);
        /*!
         * \brief Созданные объекты
         */
        static QMap<QString,BulletOgreObject*> BtOgreObjects;
        /*!
         * \brief Анимации объектов
         */
        static QList<Ogre::AnimationState*> Animations;

        const QStringList MENU_ITEMS;
    protected:
        /*!
         * \brief Создание перемещаемого объекта
         * \param node
         * \param ent
         * \return
         */
        BulletOgreObject* createBtOgreObject(Ogre::SceneNode *node,Ogre::Entity *ent);

        /*!
         * \brief создание и настройка камеры
         */
        void createCamera();
        /*!
         * \brief создание карты
         */
        void createGround();
        /*!
         * \brief создание освещения
         */
        void createLight();
        /*!
         * \brief создание меню
         */
        void createMenu();
        /*!
         * \brief создание оверлейного текстового элемента
         * \param имя элемента
         * \param содержание
         * \return элемент
         */
        Ogre::TextAreaOverlayElement*
                createTextElement(QString name,QString caption);
        /*!
         * \brief создание пешеходов
         */
        void createPedestrians();
        /*!
         * \brief создание порта вывода
         * \param окно в котором будет создан
         */
        void createViewport(Ogre::RenderWindow &rw);
        /*!
         * \brief создание оверлейных элементов
         */
        void createOverlay();
        /*!
         * \brief создание панели с текстовым элементом
         * \param имя панели
         * \param содержание текстового элемента
         * \return элемент панели
         */
        Ogre::OverlayContainer* createTextPanel(QString panelName,QString text);
        Ogre::SceneManager *mSceneMgr;
        Ogre::Camera *mCamera;
        Ogre::SceneNode *mPlayerNode;
        /*!
         * \brief физический мир
         */
        btDynamicsWorld *mPhysicWorld;
        Ogre::Overlay *mMenuOverlay;
        Ogre::Entity *mGroundEntity;
        btRigidBody *mGroundBody;
        btBvhTriangleMeshShape *mGroundShape;
};

#endif // SCENEBUILDER_H
