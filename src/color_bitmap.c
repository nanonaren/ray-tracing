#include <stdlib.h>
#include "color_bitmap.h"
#define UNUSED_ARGUMENT(x) ((void)x)

static void cbmp_init_helper( float*** c, int width, int height )
{
	int i;

	(*c) = (float**)malloc(sizeof(float*)*width);
	for (i=0; i<width; i++) {
		(*c)[i] = (float*)malloc(sizeof(float)*height);
	}
}

void cbmp_init( struct color_bitmap* cb, int width, int height )
{
	cb->width = width;
	cb->height = height;
	cbmp_init_helper(&cb->red, width, height);
	cbmp_init_helper(&cb->green, width, height);
	cbmp_init_helper(&cb->blue, width, height);
}

static void cbmp_free_helper( struct color_bitmap* cb, float*** c )
{
	int i;
	for (i = 0; i<cb->width; i++) {
		free((*c)[i]);
	}
	*c = NULL;
}

void cbmp_free( struct color_bitmap* cb )
{
	cbmp_free_helper(cb, &cb->red);
	cbmp_free_helper(cb, &cb->green);
	cbmp_free_helper(cb, &cb->blue);
}

void cbmp_start_left( struct color_bitmap* cb, struct pixel* p )
{
	UNUSED_ARGUMENT(cb);
	p->x = 0;
}

bool cbmp_at_right_end( struct color_bitmap* cb, struct pixel* p )
{
	return p->x == cb->width;
}

void cbmp_start_top( struct color_bitmap* cb, struct pixel* p )
{
	UNUSED_ARGUMENT(cb);
	p->y = 0;
}

bool cbmp_at_bottom_end( struct color_bitmap* cb, struct pixel* p )
{
	return p->y == cb->height;
}
