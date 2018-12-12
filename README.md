# PHP Extensions
The purpose of this project is to outline and show the capabilities of PHP extensions.


# Requirements
    • Linux environment
    • PHP 7.0
    • PHP 7.0 Source code (http://github.com/php/php-src/)


# Outline
## Setting up your project: 
    1. clone the php-src repo and checkout branch PHP-7.0
    2. navigate to /ext/ then run ./ext_skel –extname=name-goes-here
    3. navigate to the directory that was just created (will match whatever you put as the extname)
    4. open 'config.m4' in a text editor and paste the following (and replace any instances of EXT_NAME with your extension's name:
    
```C++
PHP_ARG_ENABLE(EXT_NAME, for EXT_NAME support,
[  --enable-EXT_NAME             Include EXT_NAME support])

if test "$PHP_EXT_NAME" != "no"; then
  PHP_REQUIRE_CXX()
  PHP_SUBST(EXT_NAME_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, EXT_NAME_SHARED_LIBADD)
  PHP_NEW_EXTENSION(EXT_NAME, EXT_NAME.cc, $ext_shared)
fi
```
	5. run the following commands in your shell:

```Shell
$ phpize
$ ./configure --enable-EXT_NAME
$ make
```
	if everything is setup properly, the `make` command will execute without any errors.

## Adding functionality to your project:
```C++
PHP_FUNCTION(function_name) {
	
}
```
