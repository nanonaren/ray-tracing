#ifndef COLOR_BITMAP_H
#define COLOR_BITMAP_H
#include <stdbool.h>

struct pixel {
	int x;
	int y;
};

struct color_bitmap {
	float** red;
	float** green;
	float** blue;
	int height;
	int width;
};

void cbmp_init( struct color_bitmap* cb, int width, int height );
void cbmp_free( struct color_bitmap* cb);
void cbmp_start_left( struct color_bitmap* cb, struct pixel* p );
bool cbmp_at_right_end( struct color_bitmap* cb, struct pixel* p );
void cbmp_start_top( struct color_bitmap* cb, struct pixel* p );
bool cbmp_at_bottom_end( struct color_bitmap* cb, struct pixel* p );

static inline void cbmp_set_color(float** c, struct pixel* p, float val)
{
	c[p->x][p->y] = val;
}

static inline void cbmp_next_row(struct pixel* p)
{
	p->y++;
}

static inline void cbmp_next_col(struct pixel* p)
{
	p->x++;
}

static inline float cbmp_get_red(struct color_bitmap* cb, struct pixel* p)
{
	return cb->red[p->x][p->y];
}

static inline float cbmp_get_green(struct color_bitmap* cb, struct pixel* p)
{
	return cb->green[p->x][p->y];
}

static inline float cbmp_get_blue(struct color_bitmap* cb, struct pixel* p)
{
	return cb->blue[p->x][p->y];
}

#endif /* COLOR_BITMAP_H */
