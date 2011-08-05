# do not use release version, there have some problem
TEMPLATE = app
TARGET = Convert2
DEPENDPATH += . debug
INCLUDEPATH += .
CONFIG += debug
QMAKE_LFAGS+= -static
MOC_DIR = ../qjdConvert2/tmp
OBJECTS_DIR = ../qjdConvert2/tmp

# Input
HEADERS += \
           qjd_convert_mainwindow.h \
           qjddatastream.h \
           qjdsegybh.h \
           qjdsegyth.h \
           qjdtraceheader.h \
    qjd.h
FORMS += qjd_convert_mainwindow.ui
SOURCES += \
           main.cpp \
           qjd_convert_mainwindow.cpp \
           qjddatastream.cpp \
           qjdsegybh.cpp \
           qjdsegyth.cpp \
           qjdtraceheader.cpp \
    qjd.cpp
RESOURCES += \
    images.qrc
