extern "C" {
	#include "php.h"
	#include "php_ini.h"

	#include "ext/standard/info.h"
	#include "image_processor.h"
}

#define ECHO(fmt, ...) //zend_printf(fmt "<br/>", __VA_ARGS__)

PHP_FUNCTION(open_file) {
	char *file_name;
	size_t file_name_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &file_name, &file_name_len) != FAILURE) {
		FILE *fp;
		if((fp = fopen(file_name, "rb")) != NULL) {
			fseek(fp, 0, SEEK_END);
			size_t file_size = ftell(fp);
			rewind(fp);

			char *buffer = (char *)malloc(sizeof(char) * file_size);
			fread(buffer, 1, file_size, fp);
			
			PHPWRITE_H(buffer, file_size);

			fclose(fp);
		} else {
		}
	} else {
	}
}

const zend_function_entry image_processor_functions[] = {
	PHP_FE(open_file, NULL)
	PHP_FE_END
};

#pragma region setup

PHP_MINIT_FUNCTION(image_processor) {
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