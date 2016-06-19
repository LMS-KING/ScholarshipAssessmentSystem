#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T14:39:41
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FollowShip
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    register.cpp \
    studentmain.cpp \
    applicationfellow.cpp \
    application.cpp \
    passwsure.cpp \
    personmessage.cpp \
    score.cpp \
    applicatermessage.cpp \
    allscoandhistory.cpp \
    fellowship.cpp \
    alldofellowship.cpp \
    classmessage.cpp \
    managermessage.cpp \
    addordelmanager.cpp \
    photo.cpp \
    dofellow.cpp \
    scoreparts.cpp

HEADERS  += login.h \
    register.h \
    studentmain.h \
    applicationfellow.h \
    application.h \
    passwsure.h \
    personmessage.h \
    score.h \
    applicatermessage.h \
    allscoandhistory.h \
    fellowship.h \
    alldofellowship.h \
    classmessage.h \
    managermessage.h \
    addordelmanager.h \
    photo.h \
    dofellow.h \
    scoreparts.h

FORMS    += login.ui \
    register.ui \
    studentmain.ui \
    applicationfellow.ui \
    application.ui \
    passwsure.ui \
    personmessage.ui \
    score.ui \
    applicatermessage.ui \
    allscoandhistory.ui \
    fellowship.ui \
    alldofellowship.ui \
    classmessage.ui \
    managermessage.ui \
    addordelmanager.ui \
    photo.ui \
    dofellow.ui \
    scoreparts.ui

RESOURCES += \
    picture.qrc

RC_FILE  += logo.rc
