QT += core
QT -= gui

CONFIG += c++11

TARGET = licensePlateCr
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    detectchars.cpp \
    preprocess.cpp \
    detectplates.cpp \
    possiblechar.cpp


INCLUDEPATH += C:/opencv/mybuild/install/include
LIBS += -LC:/opencv/mybuild/install/x86/mingw/bin\
    -lopencv_core310 \
    -lopencv_highgui310 \
    -lopencv_imgproc310 \
    -lopencv_imgcodecs310 \
    -lopencv_ml310 \

HEADERS += \
    detectplates.h \
    possibleplate.h \
    detectchars.h \
    possiblechar.h \
    preprocess.h \
    main.h \
    scalar.h
