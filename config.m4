PHP_ARG_ENABLE(image_processor, for image_processor support,
[  --enable-image_processor             Include image_processor support])

if test "$PHP_IMAGE_PROCESSOR" != "no"; then
  PHP_REQUIRE_CXX()
  PHP_SUBST(IMAGE_PROCESSOR_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, IMAGE_PROCESSOR_SHARED_LIBADD)
  PHP_NEW_EXTENSION(image_processor, image_processor.cc, $ext_shared)
fi