#!/bin/bash

OBJECTS=(Matrix33 vec3 Camera Sphere Plane Scene Image RandomSampler Rectangle)
INCLUDE=/usr/X11R6/include
LIB=/usr/X11R6/lib
FLAGS=(lm lpthread lX11 std=c++11)

clang++ main.cpp ${OBJECTS[@]/%/.cpp} -I $INCLUDE -L $LIB ${FLAGS[@]/#/-} 