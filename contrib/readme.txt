hi!  you will need to point the makefile at download/git folders for:

	BLZip 		https://github.com/BleuLlama/BLZip
	zlib-1.2.8	http://www.zlib.net/

Set the two variables to point to these directories:

	BLZIP_DIR := /Users/sdl/src/git/BLZip
	ZLIB_DIR := /Users/sdl/src/notme/zlib-1.2.8

Then type 'make' and it will build what it needs to generate libMinizip.a
