#pragma once

#include <types.h>

typedef struct _php_image {
	char *name;
	size_t name_sz;
	uint8_t *buffer;
	size_t buffer_sz;
} php_image;

static pixel *get_pixel(php_image *image, int x, int y);