#-------------------------------------------------
#
# Project created by QtCreator 2015-03-11T10:38:58
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TARGET = draw
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    drawwidget.cpp \
    picoconnection.cpp

HEADERS  += mainwindow.h \
    drawwidget.h \
    picoconnection.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources.qrc

QMAKE_POST_LINK += cp $$PWD/auto*.txt $$OUT_PWD
