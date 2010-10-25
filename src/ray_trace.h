#ifndef RAY_TRACE_H
#define RAY_TRACE_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "primitives.h"
#include "sphere.h"
#include "polygon.h"

#define MAX_LIGHTS	3
#define MAX_POLYS	10
#define MAX_SPHERES	5

/* a point light source */
typedef struct{
	float x_pos;
	float y_pos;
	float z_pos;
	float red;
	float green;
	float blue;
} Light;

/* a scene */
typedef struct{
	float view_z;
	int no_lights;
	Light lights[MAX_LIGHTS];
	int no_spheres;
	Sphere spheres[MAX_SPHERES];
	int no_polygons;
	Poly polygons[MAX_POLYS];
} SceneInfo;

#endif /* RAY_TRACE_H */
