TEMPLATE = app
CONFIG += console c++11
QMAKE_CXXFLAGS+=-std=c++14’
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

unix:!macx: LIBS += -lboost_graph

unix:!macx: LIBS += -lalpm
