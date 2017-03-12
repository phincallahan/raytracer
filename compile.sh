#!/bin/bash

OBJECTS=(Matrix33.cpp vec3.cpp Camera.cpp Sphere.cpp)
INCLUDE=/usr/X11R6/include
LIB=/usr/X11R6/lib
FLAGS=(lm lpthread lX11 std=c++11)

clang++ main.cpp ${OBJECTS[*]} -I $INCLUDE -L $LIB ${FLAGS[@]/#/-} 