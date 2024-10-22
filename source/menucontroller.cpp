#include "menucontroller.h"
#include "Ogre.h"
#include "QDebug"
#include "QProcess"
MenuController::MenuController(QStringList menuItems) :
    QObject(0),
    mMenuItems(menuItems),
    mMenuIndex(0)

{
    qDebug()<<"crete menu contr";
    enabled = true;
    //mMenuItems = menuItems;
    mMenuPointer =
            Ogre::OverlayManager::
            getSingleton().getOverlayElement("menuPointer");
    qDebug()<<"created";
}

void MenuController::keyPressed(OIS::KeyCode code) {
    static int pressTime = 0,enterPressTime = 0;
    if (!enabled) {
        return;
    }
    bool enter = false;
    switch(code) {
        case OIS::KC_UP: {
        if (pressTime<0) {
            pressTime = 0;
        }
            pressTime+=1;
        break;
    }
        case OIS::KC_DOWN: {
        if (pressTime>0) {
            pressTime = 0;
        }
                pressTime-=1;
            break;
        }
        case OIS::KC_RETURN: {
            enterPressTime+=1;
            break;
        }
    }
    //qDebug()<<pressTime;
    if (pressTime>50) {
        mMenuIndex=mMenuIndex-1;
        if (mMenuIndex<0) {
            mMenuIndex = mMenuItems.count()-1;
        }
        pressTime = 0;
    }
    qDebug()<<enterPressTime;
    if (enterPressTime>50) {
        enterPressTime = 0;
        enter = true;
    }
    if (pressTime<-50) {
        mMenuIndex=abs(mMenuIndex+1)%(mMenuItems.count());
        pressTime = 0;
    }

    //qDebug()<<"menu pointer!"<<mMenuIndex<<pressTime<<enter;
    mMenuPointer->setTop(mMenuPointer->_getTop()+mMenuIndex*40);
    if (enter) {
        switch(mMenuIndex) {
            case 0: {
            emit(start());

            enabled = false;
            break;
        }
        case 1: {
            qDebug()<<"run help"<<enter<<enterPressTime;
            QProcess::startDetached("notepad",
                                    QStringList()<<"readme.txt");

            //emit(help());
           // enterPressTime = -50;
            enabled = false;
            break;
        }
            case 2:
            emit(shutdown());
            break;
        }
    }
}

void MenuController::show() {
    enabled = true;
    Ogre::OverlayManager::
            getSingleton().getByName("MainMenu")->show();
}
void MenuController::hide() {
    enabled = false;

    Ogre::OverlayManager::
            getSingleton().getByName("MainMenu")->hide();
}
