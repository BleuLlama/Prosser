# Makefile for prosser
#

# there are a few dependancies.. 

# BLZip needs to be retrieved from github (minizip wrapper)
#	https://github.com/BleuLlama/BLZip
BLZIP_DIR := ../BLZip

# ZLib sources (v1.2.8 tested) must also be retrieved and extracted
#	http://www.zlib.net/
ZLIB_DIR := ../zlib-1.2.8


# lua 5.2.2 needs to be installed somewhere.  I installed it to 
# a directory  in my home called 'sw'. 
#	http://www.lua.org/

LUA_DIR := $(HOME)/sw


MINIZIP_DIR :=  $(ZLIB_DIR)/contrib/minizip

TARGA := prosser

SRCSCOMMON := \
	contrib/BLZip.cpp \
	contrib/BLUnZip.cpp

INCS += -Icontrib/ \
	-I/usr/include/ \
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

# for NON windows
LIBS += -ldl

################################################################################

all: $(TARGA)

%.o: %.cpp
	@echo $(CXX) $<
	@$(CXX) $(CFLAGS) $(DEFS) $(INCS) -c -o $@ $<

$(TARGA): $(OBJSA)
	@echo Link $@
	@$(CXX) $(CFLAGS) $(OBJSA) $(LDFLAGS) $(LIBS) -o $@

dist: $(TARGA) $(WADA)
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

testa: dist
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
