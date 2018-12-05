extern "C" {
	#include "php.h"
	#include "php_ini.h"
	#include "ext/standard/info.h"
	#include "image_processor.h"
}

class Image {
private:
	FILE *	__file_descriptor;
	char *	__file_path;
	size_t 	__file_size;
	
	FILE *load_image() {
		if((__file_descriptor = fopen(__file_path, "rb")) != NULL) {
			fseek(__file_descriptor, 0, SEEK_END);
			__file_size = ftell(__file_descriptor);
			rewind(__file_descriptor);
		}
		return __file_descriptor;
	}
public:
	Image(char *file_path) : __file_path(file_path) {
		if(this->load_image()) {
			zend_printf("Loaded image %s!", file_path);
		} else {
			php_error(E_ERROR, "Failed to load image %s!", file_path);
		}
	}
};

zend_class_entry *image_ce;
zend_object_handlers image_object_handlers;

struct image_object {
    zend_object std;
    Image *car;
};

void image_free_storage(void *object TSRMLS_DC)
{
    image_object *obj = (image_object *)object;
    delete obj->car;
 
    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);
 
    efree(obj);
}
 
zend_object *image_create_handler(zend_class_entry *type TSRMLS_DC) {
    zval *tmp;
    zend_object retval;
 
    image_object *obj = (image_object *)emalloc(sizeof(image_object));
    memset(obj, 0, sizeof(image_object));
    obj->std.ce = type;
 
    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(obj->std.properties, &type->default_properties_table,  (copy_ctor_func_t)zval_add_ref);
 
    retval.handle = zend_objects_store_put(obj);
    retval.handlers = &image_object_handlers;
 
    return retval;
}

PHP_METHOD(Image, __construct) {

}

const zend_function_entry image_functions[] = {
	PHP_ME(Image, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_FE_END
};

PHP_FUNCTION(open_file) {
	char *file_name;
	size_t file_name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &file_name, &file_name_len) != FAILURE) {
		FILE *fp;
		if((fp = fopen(file_name, "rb")) != NULL) {
			zend_printf("Opened file: %s", file_name);

			fseek(fp, 0, SEEK_END);
			size_t file_size = ftell(fp);
			rewind(fp);

			zend_printf("File size: %i bytes", file_size);

			char *buffer = (char *)malloc(sizeof(char) * file_size);
			fread(buffer, 1, file_size, fp);
			zend_printf("Content: %s", buffer);

			fclose(fp);
		} else {
			zend_printf("Failed to open file: %s", file_name);
		}
	} else {
		zend_printf("Missing params!");
	}
}

const zend_function_entry image_processor_functions[] = {
	PHP_FE(open_file, NULL)
	PHP_FE_END
};

#pragma region setup

PHP_MINIT_FUNCTION(image_processor) {
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Image", image_functions);
	image_ce = zend_register_internal_class(&ce TSRMLS_CC);
	image_ce->create_object = image_create_handler;

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