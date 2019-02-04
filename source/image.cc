extern "C" {
	#include "php.h"
}

#include "include/image.h"

void image_free(php_image *image) {
	if(image) {
		if(image->name) {
			efree(image->name);
		}
		if(image->buffer) {
			efree(image->buffer);
		}
		efree(image);
	}
}

pixel *get_pixel(uint8_t *buffer, int x, int y) {
	bitmap_image *bmp = ((bitmap_image *)buffer);
	return (pixel *)&bmp->pixels[(x*bmp->width + y)*3];	
}
pixel *get_pixel(php_image *image, int x, int y) {
	return get_pixel(image->buffer);
}
