#include "polygon.h"
#define TWOPI 6.283185307179586476925287
#include <stdio.h>
#include <math.h>

extern Vertex zero_vertex;

Vector polygon_normal(Poly* p, Ray* r)
{
	Vector v1 = vector_sub(&p->vertices[1], &p->vertices[0]);
	Vector v2 = vector_sub(&p->vertices[2], &p->vertices[0]);
	Vector n = vector_unit(vector_cross(&v1, &v2));
	if (vector_dot(&n, &r->dir)  > 0) {
		return scalar_mult(-1, &n);
	}
	return n;
}

static float polygon_plane_constant(Poly* p, Vector* n)
{
	return -1 * vector_dot(&p->vertices[0], n);
}

static bool point_in_interior(Poly* p, Vertex* v)
{
	Vector v1, v2;
	double angle_sum = 0;
	double m1m2;
	int i;

	for (i=0; i<p->no_vertices; i++) {
		v1 = vector_sub((Vector*)(&p->vertices[i]), v); 
		v2 = vector_sub((Vector*)(&p->vertices[(i+1)%
					p->no_vertices]), v);
		m1m2 = vector_magnitude(&v1)*vector_magnitude(&v2);
		if (m1m2 == 0) {
			return true;
		}
		angle_sum += acos(vector_dot(&v1, &v2)/m1m2);
	}
	if (TWOPI-angle_sum < 0.0001) {
		return true;
	}
	return false;
}

Vertex polygon_intersect(Ray* r, Poly* p)
{
	Vector n;
	float dotp, d, t;
	Vertex res;

	n = polygon_normal(p, r);
	dotp = vector_dot(&n, &r->dir);
	if (dotp) {
		d = polygon_plane_constant(p, &n);
		t = -1 * (vector_dot(&n, &r->origin) + d) / dotp;
		if (t < 0) {
			return zero_vertex;
		}
		res.x = r->origin.x + r->dir.x * t;
		res.y = r->origin.y + r->dir.y * t;
		res.z = r->origin.z + r->dir.z * t;

		/* now check if point is inside polygon */
		if (point_in_interior(p, &res)) {
			return res;
		}
	}
	return zero_vertex;
}
