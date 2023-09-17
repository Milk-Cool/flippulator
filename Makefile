LIBS = flipper/
EXT_LIB_ALL = lib/
#EXT_LIB_u8g2 = lib/u8g2/
HELPERS = helpers/
APP = flippulator_app_copy/
SOURCES = $(shell find $(LIBS) -name "*.c") $(shell find $(HELPERS) -name "*.c") $(shell find $(EXT_LIB_ALL) -name "*.c")
SRC_APP = $(shell find $(APP) -name "*.c")
#OBJECTS = ${subst .c,.o,$(SOURCES)}
OUT_APP = out
#OUT_LIB_NAME = flipper
#OUT_LIB = lib$(OUT_LIB_NAME).a
#CC_PREFIX = gcc -c -Wall# -fPIC
CC_PREFIX_FINAL = gcc -Wall -Wno-main
BUILD_LIB_heatshrink_PATH = lib/heatshrink/
BUILD_LIB_heatshrink = $(BUILD_LIB_heatshrink_PATH)libheatshrink_static.a $(BUILD_LIB_heatshrink_PATH)libheatshrink_dynamic.a
#CC_PREFIX = clang -c

all: $(OUT_APP)

$(OUT_APP): $(BUILD_LIB_heatshrink)
	$(CC_PREFIX_FINAL) -I$(LIBS) -I$(HELPERS) -I$(EXT_LIB_ALL) $(SRC_APP) $(SOURCES) -lSDL2 -o $(OUT_APP)

$(BUILD_LIB_heatshrink):
	make -C lib/heatshrink libraries

#$(OUT_LIB): $(OBJECTS)
#	ar rcs $(OUT_LIB) $(OBJECTS)

#$(LIBS)%.o: $(LIBS)%.c
#	$(CC_PREFIX) -I$(LIBS) -I$(HELPERS) -g $< -o $@
#$(HELPERS)%.o: $(HELPERS)%.c
#	$(CC_PREFIX) -I$(HELPERS) -g $< -o $@

clean:
	rm -rf $(OUT_APP) $(APP)