#-------------------------------------------------
#
# Project created by QtCreator 2018-02-19T14:32:41
#
#-------------------------------------------------

QT       += core opengl gui

TARGET = PhysicsEngineTP
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

LIBS += -lGLU

SOURCES += main.cpp \
    glwidget.cpp \
    particule.cpp \
    edge.cpp \
    movableactor.cpp \
    fixedactor.cpp \
    actor.cpp \
    collider.cpp

HEADERS += \
    glwidget.h \
    particule.h \
    edge.h \
    actor.h \
    movableactor.h \
    fixedactor.h \
    collider.h
