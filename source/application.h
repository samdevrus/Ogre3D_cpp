#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCoreApplication>
#include "ogreapplication.h"
/*!
 * \brief Класс основного приложения Qt
 *
 *запускает программу
 */
class Application : public QCoreApplication
{
    Q_OBJECT
    public:
        explicit Application(int &argc, char **argv);
         /*!
         * \brief цикл выполнения программы
         * \return
         */
        int exec();

    signals:

    public slots:
        /*!
         * \brief завершение работы программы
         */
        void shutdown();
    protected:
        /*!
         * \brief запускаемое приложение 3D
         */
        OgreApplication app;
};

#endif // APPLICATION_H
