######################################################################
# Automatically generated by qmake (2.01a) ven. nov. 11 16:35:15 2016
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . Unix/Headers Unix/Sources Windows/Headers Windows/Sources
INCLUDEPATH += . Windows/Headers Unix/Headers
QMAKE_CXXFLAGS += -std=c++11

# Input
HEADERS += ASocket.hh \
           Buffer.hh \
           Command.hh \
           window.h \
           Unix/Headers/LinSocket.hh
FORMS += clientbabel.ui
SOURCES += ASocket.cpp \
           Buffer.cpp \
           Command.cpp \
           main.cpp \
           window.cpp \
           Unix/Sources/LinSocket.cpp
RESOURCES += clientbabel.qrc