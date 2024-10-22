TEMPLATE = app
TARGET = CursTRPO

QT       += core gui widgets

DEFINES -= UNICODE

CONFIG += static

LIBS +=-L$(OGRE_HOME)/LIB
LIBS +=-L$(BULLET_HOME)\\lib
LIBS +=-L$(BULLET_OGRE)\\lib
release:LIBS += -L$(OGRE_HOME)\\lib\\release
debug:LIBS += -L$(OGRE_HOME)\\lib\\debug
LIBS += -L$(BOOST_ROOT)\\lib -L$(OGRE_HOME)\\lib\\RelWithDebInfo
LIBS += -lOgreMain -lOIS -lOgreTerrain
LIBS += -lboost_system-mgw46-mt-1_52
LIBS += -lBulletDynamics
LIBS += -lBulletCollision
LIBS += -lLinearMath
LIBS += -lBulletSoftBody
LIBS += -lConvexDecomposition
LIBS += -lGIMPACTUtils
LIBS += -lHACD
LIBS += -lBtOgre
LIBS += -lMiniCL
LIBS += -lBulletSoftBodySolvers_OpenCL_Mini

INCLUDEPATH += $(OGRE_HOME)\\include
INCLUDEPATH += $(OGRE_HOME)\\include\\OIS
INCLUDEPATH += $(OGRE_HOME)\\include\\OGRE
INCLUDEPATH += $(BOOST_ROOT)
INCLUDEPATH += $(BULLET_HOME)\\src
INCLUDEPATH += $(BULLET_OGRE)\\include




SOURCES += \
    main.cpp \
    ogreapplication.cpp \
    inputlistener.cpp \
    scenebuilder.cpp \
    application.cpp \
    renderframelistener.cpp \
    nodecontroller.cpp \
    playercontroller.cpp \
    BtOgre.cpp \
    bulletogreobject.cpp \
    playerobject.cpp \
    cameranodecontroller.cpp \
    pedestrianobject.cpp \
    renderwindowlistener.cpp \
    menucontroller.cpp

HEADERS += \
    ogreapplication.h \
    inputlistener.h \
    scenebuilder.h \
    application.h \
    renderframelistener.h \
    nodecontroller.h \
    playercontroller.h \
    bulletogreobject.h \
    playerobject.h \
    cameranodecontroller.h \
    pedestrianobject.h \
    renderwindowlistener.h \
    menucontroller.h
