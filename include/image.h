#pragma once

#include "stdint.h"
#include <stddef.h>

#pragma pack(push, 1)
typedef struct _bitmap_image {
    uint16_t identifier;
    uint32_t bmp_file_sz;
    uint16_t : 16;
    uint16_t : 16;
    uint32_t data_offset;
    uint32_t dib_header_sz;
    uint32_t width;
    uint32_t height;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t : 64;
    uint64_t : 64;
    uint8_t pixels[];
} bitmap_image;
#pragma pack(pop)

#pragma pack(push, 1)
typedef union _pixel {
    struct {
        uint8_t b, g, r;
    };
    uint32_t rgb : 24;
} pixel;
#pragma pack(pop)

typedef struct _php_image {
	char *name;
	size_t name_sz;
	uint8_t *buffer;
	size_t buffer_sz;
} php_image;

void image_free(php_image *image);
pixel *get_pixel(uint8_t *buffer, int x, int y);
pixel *get_pixel(php_image *image, int x, int y);