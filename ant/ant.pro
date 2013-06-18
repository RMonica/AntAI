#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T13:50:27
#
#-------------------------------------------------

QT       += core gui

TARGET = ant
TEMPLATE = app

LIBS += -lfann


SOURCES += main.cpp\
        MainWindow.cpp \
    Grid.cpp \
    GridWidget.cpp \
    Ant.cpp \
    Move.cpp \
    RecorderWidget.cpp \
    Arff.cpp \
    AntAdapter.cpp \
    AntAI.cpp \
    AntTreeAI.cpp \
    AntNeuralAI.cpp \
    MultilayerPerceptron.cpp

HEADERS  += MainWindow.h \
    Grid.h \
    defines.h \
    GridWidget.h \
    Ant.h \
    Move.h \
    Direction.h \
    GameDefines.h \
    RecorderWidget.h \
    Arff.h \
    AntAdapter.h \
    AntAI.h \
    AntTreeAI.h \
    AntNeuralAI.h \
    MultilayerPerceptron.h
