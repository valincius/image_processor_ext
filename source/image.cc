#include <types.h>

static pixel *get_pixel(php_image *image, int x, int y) {
	bitmap_image *bmp = ((bitmap_image *)image->buffer);
	return (pixel *)&bmp->pixels[(x*bmp->width + y)*3];	
}