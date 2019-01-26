# PHP Extensions
The purpose of this project is to outline and show the capabilities of PHP extensions.

# Requirements
- Linux environment
- PHP 7.0
- PHP 7.0 Dev tools

# Installing PHP + Dev tools:
```Bash
sudo apt-add-repository ppa:ondrej/php # add repo so we don't run into any issues with the next command
sudo apt-get install build-essential php7.0 php7.0-dev
```

# Testing the provided project:
## Building
1. Clone this repository
```Bash
$ git clone git@github.com:valincius/image_processor_ext.git
```

2. Run the following commands to build & install
```Bash
$ phpize
$ ./configure --enable-image_processor
$ make
$ make install
$ echo 'extension=image_processor.so' >> /etc/php/PHP_VERSION/cli/php.ini
```

3. If all of those commands run succesfully, you should now be able to use the following functions:
	- image_load
	- image_save
	- image_render
	- image_base64
	- image_apply_color
	- image_apply_mask
	- image_flip_x
	- image_flip_y
	- image_info
	- image_destroy

See ```scripts/index.php``` for example usage.

# Setting up a new project: 
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

```Bash
$ phpize
$ ./configure --enable-EXT_NAME
$ make
```

if everything is setup properly, the `make` command will execute without any errors.

## Adding functionality to your project:
In _PHP_ a simple __hello world__ function might look like this:
```PHP
function hello_person($name) {
    return 'Hello, '.$name;
}
```

We can get the same functionality in a _PHP Extension_ with the following code:
```C++
PHP_FUNCTION(hello_person) {
	char *person_str;		// Variable to store a pointer to the name
	size_t person_str_len;		// Variable to store the length of the name (not used in this case)

	/*
	* first parameter specifies the number of arguments that were passed to the function
	* next we specify what the parameters types are, in this case we only want a string, specified by 's'
	* strings are special so in this case we first pass a pointer person_str (this is where the actual string is stored) then we have another parameter that follows immediately after which is the length of the string

	if we do not get the correct parameters, the function will return false and report an error
	*/
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &person_str, &person_str_len) == FAILURE) {
		RETURN_FALSE;
	}

	// Here we set up a string then just concat the name to it, this should be familiar
	std::string output_str = "Hello, ";
	output_str += person_str;

	// Finally we return a pointer to our std string
	RETURN_STRING(output_str.c_str());
}
```

Before we can use this function, we'll have to add it to our ```zend_function_entry``` list, like so:
```C++
const zend_function_entry EXT_NAME_functions[] = {
	...
	PHP_FE(hello_person, NULL)
	...
	PHP_FE_END
	...
};
```

If the code looks good, we can rebuild the code and install it and test it out.
In your shell, run:
```Bash
$ make clean && make
```

If there are no errors then we can install and test it!
Before the first install, we will first need to enable the extension, by running the following command:
```Bash
$ echo 'extension=EXT_NAME.so' >> /etc/php/PHP_VERSION/cli/php.ini
```

We can then install and test our new function with the following commands:
```Bash
$ make install && php -r "echo hello_person('john').PHP_EOL;"
> Hello, john
```
