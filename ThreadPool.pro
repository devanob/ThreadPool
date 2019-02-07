  TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11
SOURCES += main.cpp \
    StateNode.cpp

QMAKE_LFLAGS += -pthread

HEADERS += \
    basetask.h \
    TypeTask.h \
    GenericTask.h \
    ThreadPool.h \
    ThreadSafeQueue.h \
    ThreadWorker.h \
    StateNode.h
