#ifndef PRIMITIVES_H
#define PRIMITIVES_H
#include "color_bitmap.h"
#include <stdbool.h>

enum primitve_types {
	T_SPHERE = 5,
	T_POLYGON
};

typedef struct {
	float red;
	float green;
	float blue;
} color;

/* a 3d vertex */
typedef struct{
	float x;
	float y;
	float z;
} Vertex;

typedef Vertex Vector;

typedef struct {
	Vertex topleft;
	Vertex bottomright;
} Window;

typedef struct {
	Vertex origin;
	Vector dir;
} Ray;

Vertex get_wc_pixel(Window*, struct pixel*, int, int);
Vector vector_sub(Vector* v1, Vector* v2);
Vector vector_add(Vector* v1, Vector* v2);
Vector vector_cross(Vector* v1, Vector* v2);
Vector vector_unit(Vector v);
float vector_magnitude(Vector* v);
float vector_dot(Vector* v1, Vector* v2);
bool vertex_equals(Vector* v1, Vector* v2);
float vectors_angle(Vector* v1, Vector* v2);
Vector scalar_mult(float val, Vector* v);
#endif /* PRIMITIVES_H */
