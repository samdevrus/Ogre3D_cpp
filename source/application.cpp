#include "application.h"
#include "QWidget"
#include "QDebug"
Application::Application(int &argc, char **argv):
    QCoreApplication(argc,argv)
{


}

int Application::exec() {

    try {
        app.start();
    } catch( Ogre::Exception& e ) {

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL,
                    e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " <<
            e.getFullDescription().c_str() << std::endl;
#endif
    }

    return 0;
}

void Application::shutdown() {
    app.stop();
}
