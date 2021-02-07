#-------------------------------------------------
#
# Project created by QtCreator 2014-05-15T12:49:39
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChinessChessUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ubgraphicsview.cpp \
    ubchessmanitem.cpp \
    ubgraphicsscene.cpp \
    CChessStruct.cpp \
    MoveMaker.cpp \
    UBException.cpp \
    UBString.cpp \
    ubgraphicseffect.cpp \
    ubtextvalueitem.cpp \
    ubtextcheckitem.cpp \
    ubtextlistitem.cpp \
    UBUCCI.cpp


HEADERS  += mainwindow.h \
    ubgraphicsview.h \
    Macro_Const.h \
    ubchessmanitem.h \
    ubgraphicsscene.h \
    CChessStruct.h \
    SmartPtr.h \
    MoveMaker.h \
    UBException.h \
    UBString.h \
    ubgraphicseffect.h \
    ubtextvalueitem.h \
    ubtextcheckitem.h \
    ubtextlistitem.h \
    UBUCCI.h


RESOURCES += \
    ChinessChessRes.qrc
