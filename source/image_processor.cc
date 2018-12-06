extern "C" {
#include "php.h"
#include "php_ini.h"

#include "ext/standard/info.h"
}

#include "include/image_processor.h"

#include "include/bmp.h"
#include "include/base64.h"
#include "include/image.h"

#define PHP_IMAGE_RES_NAME "Image Data"
int le_image;

PHP_FUNCTION(image_load) {
	php_image *image;

	char *file_name;
	size_t file_name_sz;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &file_name, &file_name_sz) == FAILURE) {
		RETURN_FALSE;
	}

	FILE *fp = fopen(file_name, "rb");
	if(fp == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "File does not exist.");
		RETURN_FALSE;
	}

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	rewind(fp);

	uint8_t *buffer = (uint8_t *)emalloc(sizeof(uint8_t) * file_size);
	size_t read_count = fread(buffer, 1, file_size, fp);

	image = (php_image *)emalloc(sizeof(php_image));

	image->name = estrndup(file_name, file_name_sz);
	image->name_sz = file_name_sz;

	image->buffer = buffer;
	image->buffer_sz = file_size;

	RETVAL_RES(zend_register_resource(image, le_image));

	fclose(fp);
}

PHP_FUNCTION(image_render) {
	php_image *image;
    zval *zimage;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zimage) == FAILURE) {
        RETURN_FALSE;
    }

	if((image = (php_image *)zend_fetch_resource(Z_RES_P(zimage), PHP_IMAGE_RES_NAME, le_image)) == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Not an image.");
		RETURN_FALSE;
	}
	
	PHPWRITE_H((char *)image->buffer, image->buffer_sz);
}

PHP_FUNCTION(image_base64) {
	php_image *image;
    zval *zimage;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zimage) == FAILURE) {
        RETURN_FALSE;
    }

	if((image = (php_image *)zend_fetch_resource(Z_RES_P(zimage), PHP_IMAGE_RES_NAME, le_image)) == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Not an image.");
		RETURN_FALSE;
	}
	char *image_base64_str = base64_encode((char *)image->buffer, image->buffer_sz);
	RETVAL_STR_COPY(zend_string_init(image_base64_str, strlen(image_base64_str), 0));
	efree(image_base64_str);
}

PHP_FUNCTION(image_save) {
	php_image *image;
    zval *zimage;

	char *save_file;
	size_t save_file_sz;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &zimage, &save_file, &save_file_sz) == FAILURE) {
        RETURN_FALSE;
    }

	if((image = (php_image *)zend_fetch_resource(Z_RES_P(zimage), PHP_IMAGE_RES_NAME, le_image)) == NULL) {
		RETURN_FALSE;
	}

	FILE *fp = fopen(save_file, "w");
	if(fp == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Failed to open file.");
		RETURN_FALSE;
	}
	fwrite(image->buffer, 1, image->buffer_sz, fp);
	fclose(fp);
}

PHP_FUNCTION(image_destroy) {
	php_image *image;
    zval *zimage;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zimage) == FAILURE) {
        RETURN_FALSE;
    }

	if((image = (php_image *)zend_fetch_resource(Z_RES_P(zimage), PHP_IMAGE_RES_NAME, le_image)) == NULL) {
		RETURN_FALSE;
	}

	zend_list_delete(Z_RES_P(zimage));
}

PHP_FUNCTION(image_make_green) {
	php_image *image;
    zval *zimage;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zimage) == FAILURE) {
        RETURN_FALSE;
    }

	if((image = (php_image *)zend_fetch_resource(Z_RES_P(zimage), PHP_IMAGE_RES_NAME, le_image)) == NULL) {
		RETURN_FALSE;
	}

	bitmap_image *bmp = (bitmap_image *)image->buffer;
	
	for(int x=0;x<bmp->width;x++) {
		for(int y=0;y<bmp->height;y++) {
			get_pixel(image, x, y)->g = 255;
		}
	}
}

PHP_FUNCTION(image_info) {
	php_image *image;
    zval *zimage;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zimage) == FAILURE) {
        RETURN_FALSE;
    }

	if((image = (php_image *)zend_fetch_resource(Z_RES_P(zimage), PHP_IMAGE_RES_NAME, le_image)) == NULL) {
		RETURN_FALSE;
	}

	bitmap_image *bmp = (bitmap_image *)image->buffer;

	zend_printf(
				"Path: %s<br/>"
				"Buffer size: %i bytes<br/>"
				"File size: %i bytes<br/>"
				"DIB Size: %i<br/>"
				"Height: %i pixels<br/>"
				"Width: %i pixels<br/>"
				"Pixel buffer start: 0x%X<br/>"
				"Pixel buffer end: 0x%X<br/>"
				"<br/>",

				image->name,
				image->buffer_sz,

				bmp->bmp_file_sz,

				bmp->dib_header_sz,

				bmp->height,
				bmp->width,

				&bmp->pixels[0],
				&bmp->pixels[bmp->height * bmp->width * 3]
			);

}

static void php_image_dtor(zend_resource *rimage) {
	php_image *image = (php_image *)rimage->ptr;
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

const zend_function_entry image_processor_functions[] = {
	PHP_FE(image_destroy, NULL)

	PHP_FE(image_load, NULL)
	PHP_FE(image_save, NULL)

	PHP_FE(image_render, NULL)
	PHP_FE(image_base64, NULL)

	PHP_FE(image_make_green, NULL)

	PHP_FE(image_info, NULL)

	PHP_FE_END
};

#pragma region setup

PHP_MINIT_FUNCTION(image_processor) {
	le_image = zend_register_list_destructors_ex(php_image_dtor, NULL, PHP_IMAGE_RES_NAME, module_number);
	return SUCCESS;
}
PHP_MSHUTDOWN_FUNCTION(image_processor) {
	return SUCCESS;
}
PHP_MINFO_FUNCTION(image_processor) {
	php_info_print_table_start();
	php_info_print_table_header(2, "image_processor support", "enabled");
	php_info_print_table_end();
}

zend_module_entry image_processor_module_entry = {
	STANDARD_MODULE_HEADER,
	"image_processor",
	image_processor_functions,
	PHP_MINIT(image_processor),
	PHP_MSHUTDOWN(image_processor),
	NULL,
	NULL,
	PHP_MINFO(image_processor),
	PHP_IMAGE_PROCESSOR_VERSION,
	STANDARD_MODULE_PROPERTIES};

ZEND_GET_MODULE(image_processor)

#pragma endregion