QT += core gui network widgets

HEADERS     = \
              mainwindow.h \
              voxel.h \
              readFramesFromUDP.h \
              widgetFrontFacing.h \
              widgetTopDown.h
SOURCES     = \
              main.cpp \
              mainwindow.cpp \
              readFramesFromUDP.cpp \
              widgetFrontFacing.cpp \
              widgetTopDown.cpp

# install
target.path = C:/Users/silin/Desktop/qt_usg9/usg9_ui_v0
INSTALLS += target

FORMS += \
    mainwindow.ui
