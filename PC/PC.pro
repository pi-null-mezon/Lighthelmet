#-------------------------------------------------
#
# Project created by QtCreator 2017-02-04T17:03:27
#
#-------------------------------------------------

QT += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flashmcu
TEMPLATE = app

VERSION = 1.0.0.0
DEFINES += APP_NAME=\\\"$${TARGET}\\\" \
           APP_DESIGNER=\\\"pi-null-mezon@yandex.ru\\\" \
           APP_VERSION=\\\"$${VERSION}\\\"

SOURCES += main.cpp\
        mainwindow.cpp \
    qspprocessor.cpp \
    qsignalfileparser.cpp

HEADERS  += mainwindow.h \
    qspprocessor.h \
    qsignalfileparser.h

FORMS    += mainwindow.ui

RESOURCES += resources.qrc

RC_ICONS += $${PWD}/Resources/idea.ico
