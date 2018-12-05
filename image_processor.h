#pragma once

extern "C" {
#include "php.h"
}

#define PHP_IMAGE_PROCESSOR_EXTNAME  "IMAGE PROCESSOR"
#define PHP_IMAGE_PROCESSOR_VERSION   "0.1"

extern zend_module_entry vehicles_module_entry;
#define phpext_vehicles_ptr &vehicles_module_entry;

/*
	Boolean 	b 		zend_bool
	Long 		l 		long
	Double 		d 		double
	String 		s 		char*, int
	Resource 	r 		zval*
	Array 		a 		zval*
	Object 		o 		zval*
	zval 		z 		zval*
*/
