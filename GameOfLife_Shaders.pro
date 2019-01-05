TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_RELEASE -= \
    -O \
    -O1 \
    -O2 \
    -Os \
    -march=x86-64 \
    -mtune=generic \
    -fstack-protector-strong \
    -march=generic \
    -mtune=generic\

QMAKE_CXXFLAGS_RELEASE += \
    -fno-stack-protector \
    -Os \
    -s \
    -ffast-math \
    -march=native \
    -mtune=native

LIBS+= \
    -lsfml-graphics \
    -lsfml-system \
    -lsfml-window

SOURCES += \
        main.cpp

DISTFILES += \
    vertex.glsl \
    fragment.glsl \
    display_fragment.glsl
