#ifndef SPHERE_H
#define SPHERE_H
#include "primitives.h"

typedef struct{
	float x_center;
	float y_center;
	float z_center;
	float radius;
	float red;
	float green;
	float blue;
} Sphere;

Vertex sphere_intersect(Ray* r, Sphere* s);
Vector sphere_normal(Vertex* v, Sphere* s);
#endif /* SPHERE_H */
