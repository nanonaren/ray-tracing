#ifndef POLYGON_H
#define POLYGON_H
#include "primitives.h"

typedef struct{
	int no_vertices;
	Vertex *vertices;
	float red;
	float green;
	float blue;
} Poly;

Vector polygon_normal(Poly* p, Ray* r);
Vertex polygon_intersect(Ray* r, Poly* p);
#endif /* POLYGON_H */
