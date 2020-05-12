#pragma once
#include "Geometry.h"

#include "MicroGlut.h"
#if defined(_WIN32)
#include "glew.h"
#endif
#include <GL/gl.h>
#include "GL_utilities.h"
#include <math.h>


Geometry* DemoOne();
Geometry* DemoTwo();
Geometry* DemoThree();
