## Overview
cs-311-raytracer is a simple implementation of the Whitted ray-tracing algorithm. In its current state, it is capable of drawing a .png image file containing spheres and planes with realistic lighting. Currently implemented are simple diffuse and specular lighting, as well as reflection and refraction. For additional modeling uses, the implementation includes a camera which allows for rotating the viewing volume of the scene.

### Organization
The project contains a main file which sets up objects and their material properties and writes an image to a .png in the current directory. Included in this file are functions for tracing rays through a scene, as well as functions which perform reflection and refraction calculations. All other files in the project are simple class files which implement objects that we have created, such as spheres, planes, and lights. 

### Installation and Execution
If on a CS lab machine, either clone this repository or obtain a zip of the source code. This can be done in a terminal using:
```
git clone https://github.com/phincallahan/cs-311-raytracer.git
```
Navigate to the directory containing all source files and run the provided compile script:
```
bash compile.sh
```
Finally, execute the created executable with:
```
./a.out
```
The executable will produce a .png file called *raytrace.png* in the local directory.

