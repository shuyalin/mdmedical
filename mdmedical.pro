#-------------------------------------------------
#
# Project created by QtCreator 2018-03-09T11:18:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = mdmedical
TEMPLATE = app

INCLUDEPATH += /home/root/mdworkdir/xlslib_arm/xlslib/tmp/include/ \

SOURCES += main.cpp\
        mdmedical.cpp \
    qcustomplot.cpp \
    serial.cpp \
    cam.cpp \
    managerdialog.cpp

HEADERS  += mdmedical.h \
    qcustomplot.h \
    serial.h \
    cam.h \
    managerdialog.h \


FORMS    += mdmedical.ui \
    managerdialog.ui

LIBS += -L /home/root/mdworkdir/xlslib_arm/xlslib/tmp/lib/  -lxls
