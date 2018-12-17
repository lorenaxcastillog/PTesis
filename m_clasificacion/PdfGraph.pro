#-------------------------------------------------
#
# Project created by QtCreator 2015-08-04T22:58:28
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = PdfGraph
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp

SOURCES += main.cpp \
    porter2_stemmer.cpp

HEADERS += \
    porter2_stemmer.h \
    grafopdf.h \
    nodo_grafo.h \
    Dictionary.h
