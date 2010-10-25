#include "sphere.h"
#include <math.h>

extern Vertex zero_vertex;

Vertex sphere_intersect(Ray* r, Sphere* s)
{
	Vertex ret;
	float a, b, c, d, t0, t1, tf;

	a = r->dir.x * r->dir.x + r->dir.y * r->dir.y +
			r->dir.z * r->dir.z;
	b = 2*(r->dir.x * (r->origin.x - s->x_center) +
			r->dir.y * (r->origin.y - s->y_center) +
			r->dir.z * (r->origin.z - s->z_center));
	c = (r->origin.x - s->x_center) * (r->origin.x - s->x_center)
		+ (r->origin.y - s->y_center) * (r->origin.y - s->y_center)
		+ (r->origin.z - s->z_center) * (r->origin.z - s->z_center)
		- s->radius * s->radius;

	d = b*b - 4*a*c;
	
	if (d < 0) {
		/* ray does not intersect */
		return zero_vertex;
	}
	else {
		t0 = (-b - sqrt(d))/2;
		if (t0 < 0) {
			t1 = (-b + sqrt(d))/2;
			if (t1 < 0) {
				/* ray does not intersect */
				return zero_vertex;
			}
			tf = t1;
		}
		else {
			tf = t0;
		}
		/*calculate intersection*/
		ret.x = r->origin.x + r->dir.x * tf;
		ret.y = r->origin.y + r->dir.y * tf;
		ret.z = r->origin.z + r->dir.z * tf;
	}
	return ret;
}

Vector sphere_normal(Vertex* v, Sphere* s)
{
	Vector res = { (v->x - s->x_center)/s->radius,
			(v->y - s->y_center)/s->radius,
			(v->z - s->z_center)/s->radius };
	return res;
}
