#-------------------------------------------------
#
# Project created by QtCreator 2016-03-29T00:27:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qpdfwalker
TEMPLATE = app

CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14

INCLUDEPATH += /usr/include/poppler

SOURCES += main.cpp\
        maindialog.cpp \
    walkerwindow.cpp \
    pdfwalker.cpp \
    pdfdataview.cpp \
    pdfwalkerobject.cpp \
    pdfwalkerdictionary.cpp \
    pdfwalkername.cpp \
    pdfwalkerarray.cpp \
    pdfwalkerstring.cpp \
    pdfwalkerboolean.cpp \
    pdfwalkerstream.cpp \
    streamdatadialog.cpp

HEADERS  += maindialog.h \
    walkerwindow.h \
    pdfwalker.h \
    pdfdataview.h \
    global.h \
    pdfwalkerobject.h \
    pdfwalkerdictionary.h \
    pdfwalkername.h \
    pdfwalkerarray.h \
    pdfwalkerstring.h \
    pdfwalkernumber.h \
    pdfwalkerboolean.h \
    pdfwalkerstream.h \
    streamdatadialog.h

FORMS    += maindialog.ui \
    walkerwindow.ui \
    streamdatadialog.ui

LIBS += -lpoppler
