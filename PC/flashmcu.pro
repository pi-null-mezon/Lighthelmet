#-------------------------------------------------
#
# Project created by QtCreator 2017-02-04T17:03:27
#
#-------------------------------------------------

QT += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flashmcu
TEMPLATE = app

VERSION = 1.0.0.2
DEFINES += APP_NAME=\\\"$${TARGET}\\\" \
           APP_DESIGNER=\\\"Alex_A._Taranov\\\" \
           APP_VERSION=\\\"$${VERSION}\\\" \
           APP_WEB=\\\"https://github.com/pi-null-mezon/lighthelmet.git\\\" \
           APP_COMPANY=\\\"МГТУ\\\" \
           APP_YEAR=\\\"2017\\\"

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

CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
