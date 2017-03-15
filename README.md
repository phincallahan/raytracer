## Overview
cs-311-raytracer is a simple implementation of the Whitted ray-tracing algorithm. In its current state, it is capable of drawing a .png image file containing spheres and planes with realistic lighting. Currently implemented are simple diffuse and specular lighting, as well as reflection and refraction. For additional modeling uses, the implementation includes a camera which allows for rotating the viewing volume of the scene.

#### Further Work
Currently our reflective lighting algorithm produces an unrealistic visual effect in our spheres when they are looked at from a shadowed perspective. Currently, we utilize a color mixing algorithm that adds local lighting to the reflected and refracted light colors (ratios found from fresnel) for every pixel to draw our spheres. The local lighting calculation takes into account the light from the sphere when returning a color. In the case of a shadow, however, the local lighting function will return black, which will then be added to reflection and refraction colors. Because a reflection can still return colors while in a shadowed area, the shadows in our reflective objects act as perfect mirrors. The reason for this lies in the way we recursively trace rays at points of reflection. When we trace reflection rays, we trace a ray that is assumed to be white light. However, if an object is a color other than white, then the object will absorb all light rays other than its color. If we could alter our rays to be a spectrum of colors and simply trace the color of the reflective object when we calculate our reflection rays, we would achieve a more realistic effect -- the objects seen in the reflection would be tinted a color consistent with the reflective sphere's color.

### Organization
The project contains a main file which sets up objects and their material properties and writes an image to a .png in the current directory. Included in this file are functions for tracing rays through a scene, as well as functions which perform reflection and refraction calculations. All other files in the project are simple class files which implement objects that we have created, such as spheres, planes, and lights. 

### Installation and Execution
To execute, either clone this repository or obtain a zip of the source code. This can be done in a terminal using:
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


