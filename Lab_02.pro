#-------------------------------------------------
#
# Project created by QtCreator 2012-09-18T17:01:09
#
#-------------------------------------------------

QT       += core gui opengl
QMAKE_CXXFLAGS += -std=c++11
DEFINES += GL_GLEXT_PROTOTYPES
LIBS += -lassimp

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab_02
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    objects/globject.cpp \
    texturemanager.cpp \
    camera/camera.cpp \
    glpainter.cpp \
    objects/assimpmodel.cpp \
    camera/freelookcamera.cpp \
    scene.cpp \
    lightsource.cpp \
    objects/group.cpp \
    texture.cpp \
    transformable.cpp \
    material.cpp \
    objects/axes.cpp \
    rect.cpp \
    objects/sphere.cpp \
    texturepainter.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    objects/globject.h \
    texturemanager.h \
    camera/camera.h \
    glpainter.h \
    objects/assimpmodel.h \
    camera/freelookcamera.h \
    scene.h \
    lightsource.h \
    objects/group.h \
    texture.h \
    transformable.h \
    material.h \
    objects/axes.h \
    rect.h \
    objects/sphere.h \
    texturepainter.h

RESOURCES += \
    resources.qrc \
    shaders.qrc

OTHER_FILES += \
    shaders/f_main.frag \
    shaders/v_main.vert \
    shaders/mtex_landscape.frag \
    shaders/mtex_landscape.vert \
    shaders/single_tex.frag \
    shaders/single_tex.vert

