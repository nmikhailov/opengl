#-------------------------------------------------
#
# Project created by QtCreator 2012-09-18T17:01:09
#
#-------------------------------------------------

QT       += core gui opengl
LIBS    += -lGLU
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab_02
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    terragen/terragen.cpp \
    terragen/randomterragen.cpp \
    objects/globject.cpp \
    objects/landscape.cpp \
    terragen/diamondsquaregen.cpp \
    terragen/spheregen.cpp \
    texturemanager.cpp \
    objects/camera.cpp \
    model/model.cpp \
    model/objmodel.cpp \
    model/glm.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    terragen/terragen.h \
    terragen/randomterragen.h \
    objects/globject.h \
    objects/landscape.h \
    objects/coloring/coloringmodel.h \
    terragen/diamondsquaregen.h \
    terragen/spheregen.h \
    texturemanager.h \
    objects/camera.h \
    model/model.h \
    model/objmodel.h \
    model/glm.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc



ParentDirectory = "/tmp/qt/lab_22"

RCC_DIR = "$$ParentDirectory/Build/RCCFiles"
UI_DIR = "$$ParentDirectory/Build/UICFiles"
MOC_DIR = "$$ParentDirectory/Build/MOCFiles"
OBJECTS_DIR = "$$ParentDirectory/Build/ObjFiles"

CONFIG(debug, debug|release) {
    DESTDIR = "$$ParentDirectory/debug"
}
CONFIG(release, debug|release) {
    DESTDIR = "$$ParentDirectory/release"
}

