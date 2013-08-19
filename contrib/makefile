
BLZIP_DIR := /Users/sdl/src/git/BLZip
ZLIB_DIR := /Users/sdl/src/notme/zlib-1.2.8
MINIZIP_DIR :=  $(ZLIB_DIR)/contrib/minizip


all:  libMinizip.a BLZip.h BLZip.cpp BLUnZip.cpp BLUnZip.h

################################################################################


libz.a:
	@echo ================== ZLIB ==================
	@cd zlib-1.2.8 ; ./configure ; gmake ; cp libz.a ..


################################################################################

MINIZIP_SRCS := \
		$(MINIZIP_DIR)/ioapi.c \
		$(MINIZIP_DIR)/unzip.c \
		$(MINIZIP_DIR)/zip.c

CFLAGS += -I$(MINIZIP_DIR) -I$(ZLIB_DIR) 

MINIZIP_OBJS := $(MINIZIP_SRCS:%.c=%.o)

libMinizip.a: $(MINIZIP_OBJS)
	@echo ================== Minizip ==================
	ar rcs $@ $(MINIZIP_OBJS)
	ranlib $@	

clean:
	rm libMinizip.a
	rm BLZip.h BLZip.cpp BLUnZip.h BLUnZip.cpp

################################################################################

BLZip.h BLZip.cpp BLUnZip.h BLUnZip.cpp:
	cp $(BLZIP_DIR)/src/$@ .
