TEMPLATE = app
CONFIG += console c++11
QMAKE_CXXFLAGS+=-std=c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    package_dependency.cpp \
    alpm_manager.cpp \
    graph_manager.cpp

unix:!macx: LIBS += -lboost_graph

unix:!macx: LIBS += -lalpm

HEADERS += \
    package_dependency.h \
    alpm_manager.h \
    graph_manager.h
