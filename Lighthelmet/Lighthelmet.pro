#-------------------------------------------------
#
# Project created by QtCreator 2018-06-22T09:23:24
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lighthelmet
TEMPLATE = app
VERSION = 1.0.0.1

DEFINES += APP_NAME=\\\"$${TARGET}\\\" \
            APP_VERSION=\\\"$${VERSION}\\\" \
            APP_DESIGNER=\\\"Alex_A._Taranov\\\"

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        qcolorshowwidget.cpp \
        $${PWD}/../PC/QSignalFileParser/qsignalfileparser.cpp \
    qcolorgenerator.cpp


HEADERS += \
        mainwindow.h \
        qcolorshowwidget.h \
        $${PWD}/../PC/QSignalFileParser/qsignalfileparser.h \
    qcolorgenerator.h

INCLUDEPATH += $${PWD}/../PC/QSignalFileParser

FORMS += \
        mainwindow.ui

RESOURCES += $${TARGET}.qrc

RC_ICONS += $${PWD}/Resources/Images/$${TARGET}.ico

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

