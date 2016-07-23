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

SOURCES += src/main.cpp\
        src/maindialog.cpp \
    src/walkerwindow.cpp \
    src/pdfwalker.cpp \
    src/pdfdataview.cpp \
    src/pdfwalkerobject.cpp \
    src/pdfwalkerdictionary.cpp \
    src/pdfwalkername.cpp \
    src/pdfwalkerarray.cpp \
    src/pdfwalkerstring.cpp \
    src/pdfwalkerboolean.cpp \
    src/pdfwalkerstream.cpp \
    src/streamdatadialog.cpp \
    src/scrollareaviewport.cpp \
    src/aboutdialog.cpp

HEADERS  += src/maindialog.h \
    src/walkerwindow.h \
    src/pdfwalker.h \
    src/pdfdataview.h \
    src/global.h \
    src/pdfwalkerobject.h \
    src/pdfwalkerdictionary.h \
    src/pdfwalkername.h \
    src/pdfwalkerarray.h \
    src/pdfwalkerstring.h \
    src/pdfwalkernumber.h \
    src/pdfwalkerboolean.h \
    src/pdfwalkerstream.h \
    src/streamdatadialog.h \
    src/scrollareaviewport.h \
    src/aboutdialog.h

FORMS    += src/maindialog.ui \
    src/walkerwindow.ui \
    src/streamdatadialog.ui \
    src/aboutdialog.ui

LIBS += -lpoppler

RESOURCES += \
    src/resources.qrc

INSTALLS += target
