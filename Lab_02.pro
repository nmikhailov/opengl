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
    terragen/terragen.cpp \
    terragen/randomterragen.cpp \
    objects/globject.cpp \
    objects/landscape.cpp \
    terragen/diamondsquaregen.cpp \
    terragen/spheregen.cpp \
    texturemanager.cpp \
    matrixstackmanager.cpp \
    camera/lookatcamera.cpp \
    camera/camera.cpp \
    contextmanager.cpp \
    shaders/shaderprogram.cpp \
    shaders/shadermanager.cpp \
    shaders/colorshader.cpp \
    shaders/textureblendshader.cpp \
    objects/assimpmodel.cpp \
    objects/axis.cpp \
    camera/freelookcamera.cpp

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
    matrixstackmanager.h \
    camera/lookatcamera.h \
    camera/camera.h \
    contextmanager.h \
    shaders/shaderprogram.h \
    shaders/shadermanager.h \
    shaders/colorshader.h \
    shaders/textureblendshader.h \
    objects/assimpmodel.h \
    objects/axis.h \
    camera/freelookcamera.h

RESOURCES += \
    resources.qrc \
    shaders.qrc

OTHER_FILES += \
    shaders/f_main.frag \
    shaders/v_main.vert \
    shaders/mtex_landscape.frag \
    shaders/mtex_landscape.vert

