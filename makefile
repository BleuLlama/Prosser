# Makefile for msf-dump
#
BLZIP_DIR := /Users/sdl/src/git/BLZip
ZLIB_DIR := /Users/sdl/src/notme/zlib-1.2.8
MINIZIP_DIR :=  $(ZLIB_DIR)/contrib/minizip
LUA_DIR := /Users/sdl/sw

TARGA := prosser

SRCSCOMMON := \
	contrib/BLZip.cpp \
	contrib/BLUnZip.cpp

INCS += -Icontrib/ \
	-I$(ZLIB_DIR) \
	-I$(MINIZIP_DIR) \
	-I$(LUA_DIR)/include/


SRCSA := $(SRCSCOMMON) \
	src/BLua.cpp \
	src/Shell.cpp \
	src/ProsserShell.cpp \
	src/Utils.cpp \
	src/StringUtils.cpp \
	src/main.cpp 

INCS += -Isrc -Icontrib

OBJSA := $(SRCSA:%.cpp=%.o)

WADA := wad.zip

CFLAGS += -g

xx += -Wall -pedantic

LDFLAGS += -Lcontrib -L$(LUA_DIR)/lib/
LIBS += -lMinizip -lz -llua

################################################################################

all: $(TARGA)

%.o: %.cpp
	@echo $(CXX) $<
	@$(CXX) $(CFLAGS) $(DEFS) $(INCS) -c -o $@ $<

$(TARGA): $(OBJSA) $(WADA)
	@echo Link $@
	@$(CXX) $(CFLAGS) $(OBJSA) $(LDFLAGS) $(LIBS) -o $@
	@echo appending WAD ZIP file to EXE
	@cat $(WADA) >> $(TARGA)
	@rm $(WADA)

################################################################################

$(WADA):
	@echo Building data WAD
	cd data ; zip -rp ../$@ *

clean:
	@echo Remove build files
	-rm -f $(OBJSA) $(TARGA) $(TARGA).exe $(WADA)
.PHONY: clean

clobber: clean
	@echo Remove library and build files
	-cd contrib ; make clean
.PHONY: clobber

kickstart:
	-cd contrib ; make
.PHONY: kickstart

testa: $(TARGA)
	./$(TARGA) -wizard
.PHONY: testa

testn: $(TARGA)
	./$(TARGA)
.PHONY: testn

test: testa
.PHONY: test


testfail: $(TARGA)
	cd live ; ../$(TARGA)
.PHONY: testfail

full: clean
	make test
.PHONY: full
