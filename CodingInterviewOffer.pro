QT += core
QT -= gui

CONFIG += c++11

TARGET = CodingInterviewOffer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    utilities.cpp

LIBS+=-lglog

HEADERS += \
    utilities.h \
    Questions.hpp \
    mygloger.hpp
