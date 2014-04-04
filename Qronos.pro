#-------------------------------------------------
#
# Project created by QtCreator 2014-03-29T18:18:17
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qronos
TEMPLATE = app


SOURCES += main.cpp\
        qronos.cpp \
    database.cpp \
    addprojectdialog.cpp \
    trackingmodel.cpp

HEADERS  += qronos.h \
    database.h \
    addprojectdialog.h \
    trackingmodel.h

FORMS    += qronos.ui \
    addprojectdialog.ui

RESOURCES += \
    images.qrc
