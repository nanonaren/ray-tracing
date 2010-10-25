#include "primitives.h"
#include <math.h>

Vertex zero_vertex = {99999999, 99999999, 99999999};

Vertex get_wc_pixel(Window* w, struct pixel* p, int screenw, int screenh)
{
	Vertex ret = { w->topleft.x + (w->bottomright.x - 
			w->topleft.x)*(p->x)/screenw,
			w->bottomright.y + (w->topleft.y -
			w->bottomright.y)*(p->y)/screenh, w->topleft.z };
	return ret;
}

Vector vector_sub(Vector* v1, Vector* v2)
{
	Vector ret = { v1->x - v2->x,
			v1->y - v2->y,
			v1->z - v2->z };
	return ret;
}

Vector vector_cross(Vector* v1, Vector* v2)
{
	Vector ret = { v1->y * v2->z - v1->z * v2->y,
			v1->z * v2->x - v1->x * v2->z,
			v1->x * v2->y - v1->y * v2->x };
	return ret;
}

float vector_magnitude(Vector* v)
{
	return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

Vector vector_unit(Vector v)
{
	float mag = vector_magnitude(&v);
	Vector ret = { v.x/mag, v.y/mag, v.z/mag };
	return ret;
}

float vector_dot(Vector* v1, Vector* v2)
{
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

bool vertex_equals(Vector* v1, Vector* v2)
{
	return v1->x == v2->x && v1->y == v2->y && v1->z == v2->z;
}

float vectors_angle(Vector* v1, Vector* v2)
{
	float val = vector_dot(v1, v2);
	return acos(val/(vector_magnitude(v1)*vector_magnitude(v2)));
}

Vector vector_add(Vector* v1, Vector* v2)
{
	Vector res = { v1->x + v2->x,
			v1->y + v2->y,
			v1->z + v2->z };
	return res;
}

Vector scalar_mult(float val, Vector* v)
{
	Vector res = { v->x * val,
			v->y * val,
			v->z * val };
	return res;
}
