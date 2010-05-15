/*****************************
 ray_trace.c

 This file contains skeleton code for the ray tracing
 assignment.  You need to write the code to ray trace
 the scene read in from the input file.
*******************************/

#include "ray_trace.h"
#include "color_bitmap.h"

extern void readSceneFile(char *filename);

/* the information read from the file */
SceneInfo scene;

/* keep low while testing, raise to see high quality image */
int win_size = 400;

/*the dimensions of the world window*/
Window world_w;

extern Vertex zero_vertex;

/* to create an ordering of the objects */
typedef struct order_t {
	void* obj;
	int type;
	float depth;
	struct order_t* next;
} order;

/******************the lighting parameters*************** */
static float diffuse_coeff = 0.8;
static float absorb_coeff = 0.4;
static float shine = 50;
static float ambient_coeff = 0.3;
static float ambient_inten = 0.3;
/******************end lighting parameters*************** */

/* some helper methods */
static Vertex intersect_with(order* obj, Ray* r)
{
	switch (obj->type) {
	case T_SPHERE:
		return sphere_intersect(r, obj->obj);
	case T_POLYGON:
		return polygon_intersect(r, obj->obj);
	}
}

/* a has one common point with n */
static Vector reflected_vector(Ray* r, Vector* n)
{
	Vector add1 = scalar_mult(2*vector_dot(n, &r->dir), n);
	Vector res = vector_sub(&add1, &r->dir);
	return res;
}

static bool ray_is_blocked(order* start, Ray* r, order* current)
{
	order* item = start;
	Vertex inter;

	while (item) {
		if (item == current) {
			item = item->next;
			continue;
		}
		inter = intersect_with(item, r);
		if (!vertex_equals(&inter, &zero_vertex)) {
			return true;
		}
		item = item->next;
	}
	return false;
}

static color illuminate(Light* l, Ray* r, Vertex* p, Vector* n, order* start,
				order* current)
{
	Ray ref_ray, shadow_ray;
	float angle, coeff;
	bool light_blocked;
	color res = {0, 0, 0};
	Vertex lightv = {l->x_pos, l->y_pos, l->z_pos};

	shadow_ray.dir = vector_sub(&lightv, p);
	shadow_ray.dir = vector_unit(shadow_ray.dir);
	shadow_ray.origin = *p;
	/*diffuse lighting*/
	/*check if the light ray is blocked*/
	light_blocked = ray_is_blocked(start, &shadow_ray, current);
	if (!light_blocked) {
		angle = vector_dot(n, &shadow_ray.dir);
		coeff = diffuse_coeff * angle;
		res.red = l->red * coeff;
		res.green = l->green * coeff;
		res.blue = l->blue * coeff;
	}

	/*specular lighting*/
	ref_ray.dir = reflected_vector(&shadow_ray, n);
	ref_ray.origin = *p;
	if (!light_blocked) {
		angle = vector_dot(&r->dir, &ref_ray.dir);
		coeff = absorb_coeff * pow(angle, shine);
		res.red += l->red * coeff;
		res.green += l->green * coeff;
		res.blue += l->blue * coeff;
	}

	return res;
}

void insert_in_order(order** start, void* obj, float depth, int type)
{
	order* item = *start;
	order* prev = NULL;
	order* new_item = (order*)malloc(sizeof(order));;
	new_item->obj = obj;
	new_item->depth = depth;
	new_item->type = type;
	new_item->next = NULL;

	while(item != NULL) {
		if (depth > item->depth) {
			new_item->next = item;
			if (prev) {
				prev->next = new_item;
				return ;
			}
			else {
				*start = new_item;
				return ;
			}

		}
		prev = item;
		item = item->next;
	}
	/* This means this goes in the end */
	/*if the list was not empty*/
	if (prev) {
		prev->next = new_item;
	}
	else {
		*start = new_item;
	}
}

/* order objects by their z depth farthest first
 * all the depths given are negative*/
static order* order_objects()
{
	int i;
	int k;
	float smallestz = 99999;
	int smallesti = 0;
	order* start = NULL;
	/*order spheres*/
	for (i=0; i<scene.no_spheres; i++) {
		insert_in_order(&start, &scene.spheres[i], 
				scene.spheres[i].z_center, T_SPHERE);
	}

	/*order polygons*/
	for (i=0; i<scene.no_polygons; i++) {
		/* find the vertex with the smallest z*/
		for (k=0; k<scene.polygons[i].no_vertices; k++) {
			if (scene.polygons[i].vertices[k].z < smallestz) {
				smallestz = scene.polygons[i].vertices[k].z;
				smallesti = k;
			}
		}	
		insert_in_order(&start, &scene.polygons[i],
			scene.polygons[i].vertices[smallesti].z, T_POLYGON);
	}
	return start;
}

static color perform_intersects(order* start, Ray* r, Light* l)
{
	order* item = start;
	Vertex v;
	Vector n;
	color c = {0, 0, 0};
	float closestz = -99999;

	while (item) {
		switch (item->type) {
			case T_SPHERE:
				v = intersect_with(item, r);
				if (vertex_equals(&v, &zero_vertex))
					break;
				if (v.z > closestz) {
					closestz = v.z;
				n = sphere_normal(&v, item->obj);
				c = illuminate(l, r, &v, &n, start, item);
				c.red *= ((Sphere*)item->obj)->red;
				c.green *= ((Sphere*)item->obj)->green;
				c.blue *= ((Sphere*)item->obj)->blue;
				}
				break;
			case T_POLYGON:
				v = intersect_with(item, r);
				if (vertex_equals(&v, &zero_vertex))
					break;
				if (v.z > closestz) {
					closestz = v.z;
				n = polygon_normal(item->obj, r);
				c = illuminate(l, r, &v, &n, start, item);
				c.red *= ((Poly*)item->obj)->red;
				c.green *= ((Poly*)item->obj)->green;
				c.blue *= ((Poly*)item->obj)->blue;
				}
				break;
			default:
				printf("not a valid object type\n");
				exit(-1);
		}
		item = item->next;
	}
	return c;
}

/*
*  rayTrace
*  This is the function that raytraces the given scene.  It
*  should calculate the color value of each pixel and draw
*  it in the interface window.
*/
void rayTrace(void)
{
	struct color_bitmap cbmp;
	struct pixel p;
	float rcolor = 0.0;
	float gcolor = 0.0;
	float bcolor = 0.0;
	order* objects = NULL;
	Vector eye = {0, 0, scene.view_z};
	Ray p_ray;

	glClear(GL_COLOR_BUFFER_BIT);
	
	/* allocate memory for our local buffer */
	cbmp_init(&cbmp, win_size, win_size);

	/*order the objects*/
	objects = order_objects();

	for(cbmp_start_left(&cbmp, &p); 
		!cbmp_at_right_end(&cbmp, &p); 
		cbmp_next_col(&p)) {

		for(cbmp_start_top(&cbmp, &p); 
			!cbmp_at_bottom_end(&cbmp, &p); 
			cbmp_next_row(&p)) {
			/* determine the world coordinate 
			 * location of the pixel */
			Vertex ploc = get_wc_pixel(&world_w, &p, 
				cbmp.width, cbmp.height);

			/* and create the primary ray */
			p_ray.origin = eye;
			p_ray.dir = vector_unit(vector_sub(&ploc, &eye));

			/* for each object, determine if the 
			 * primary ray intersects the object; 
			 * keep track of the nearest intersected object */
			rcolor = 0; gcolor = 0; bcolor = 0;
			int l;
			for (l=0; l<scene.no_lights; l++) {
				color c = perform_intersects(objects, &p_ray, 
							&scene.lights[0]);
				/*add ambient lighting*/
				rcolor += c.red + ambient_coeff*ambient_inten;
				gcolor += c.green + ambient_coeff*ambient_inten;
				bcolor += c.blue + ambient_coeff*ambient_inten;
			}

			/* set the color for the pixel in our local buffer */
			cbmp_set_color(cbmp.red, &p, rcolor);
			cbmp_set_color(cbmp.green, &p, gcolor);
			cbmp_set_color(cbmp.blue, &p, bcolor);
		}
	}

	/* Now draw all the pixels */
	glBegin(GL_POINTS);
	for(cbmp_start_left(&cbmp, &p); 
		!cbmp_at_right_end(&cbmp, &p); 
		cbmp_next_col(&p)) {
		for(cbmp_start_top(&cbmp, &p); 
			!cbmp_at_bottom_end(&cbmp, &p); 
			cbmp_next_row(&p)) {
			glColor3f(	cbmp_get_red(&cbmp, &p), 
					cbmp_get_green(&cbmp, &p), 
					cbmp_get_blue(&cbmp, &p));
			glVertex2i(p.x, p.y);
		}
	}
	cbmp_free(&cbmp);
	glEnd();
	glFlush();
}

void reshape(int w, int h)
{
	win_size = w;
}

/*
 * Main function.  Reads in the scene information,
 * places it in data structures, and creates an interface window.
 */
int main(int argc, char **argv)
{
	/* initialize */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

	/* read input */
	readSceneFile(argv[1]);

	/* create the interface window */
	glutInitWindowSize(win_size, win_size);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Ray Tracing");
	gluOrtho2D(0.0, win_size, 0.0, win_size);

	/* set the display function */
	glutDisplayFunc(rayTrace);
	glutReshapeFunc(reshape);

	/*set up some global vars*/
	world_w.topleft.x = -1;
	world_w.topleft.y = 1;
	world_w.topleft.z = 0;
	world_w.bottomright.x = 1;
	world_w.bottomright.y = -1;
	world_w.bottomright.z = 0;

	/* enter the main loop */
	glutMainLoop();
	return 0;
}

