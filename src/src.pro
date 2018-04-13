TEMPLATE = app
TARGET = goPage
DEPENDPATH += .
INCLUDEPATH += .

QMAKE_CXXFLAGS += -std=c++11
CONFIG += debug

# Input
HEADERS += monitor.hpp rmp.hpp pdfViewer.hpp manager.hpp
SOURCES += goPage.cpp monitor.cpp rmp.cpp pdfViewer.cpp manager.cpp
