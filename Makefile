LIBS = flipper/
LIBS_HAL = flipper_hal/
EXT_LIB_ALL = lib/
#EXT_LIB_u8g2 = lib/u8g2/
HELPERS = helpers/
APP = flippulator_app_copy/
SOURCES = $(shell find $(LIBS) -name "*.c") $(shell find $(LIBS_HAL) -name "*.c") $(shell find $(HELPERS) -name "*.c") $(shell find $(EXT_LIB_ALL) -name "*.c")
SRC_APP = $(shell find $(APP) -name "*.c")
#OBJECTS = ${subst .c,.o,$(SOURCES)}
OUT_DIR = out/
OUT_APP = $(OUT_DIR)out
SRC_FONT = haxrcorp-4089.ttf
OUT_FONT = $(OUT_DIR)haxrcorp-4089.ttf
OUT_CR = $(OUT_DIR)copyright.txt
OUT_ALL = $(OUT_APP) $(OUT_FONT)
#OUT_LIB_NAME = flipper
#OUT_LIB = lib$(OUT_LIB_NAME).a
#CC_PREFIX = gcc -c -Wall# -fPIC
CC_PREFIX_FINAL = gcc -Wall -Wno-main
BUILD_LIB_heatshrink_PATH = lib/heatshrink/
BUILD_LIB_heatshrink = $(BUILD_LIB_heatshrink_PATH)libheatshrink_static.a $(BUILD_LIB_heatshrink_PATH)libheatshrink_dynamic.a
#CC_PREFIX = clang -c
CR_TEXT = \
	"The FontStruction “HaxrCorp 4089” (https://fontstruct.com/fontstructions/show/192981) by “sahwar” is licensed under a Creative Commons Attribution Share Alike license (http://creativecommons.org/licenses/by-sa/3.0/)."

all: $(OUT_ALL)

$(OUT_ALL): $(OUT_APP) $(SRC_FONT)
	cp $(SRC_FONT) $(OUT_FONT)
	echo $(CR_TEXT) > $(OUT_CR)

$(OUT_APP): $(BUILD_LIB_heatshrink)
	mkdir $(OUT_DIR)
	$(CC_PREFIX_FINAL) -I$(LIBS) -I$(LIBS_HAL) -I$(HELPERS) -I$(EXT_LIB_ALL) $(SRC_APP) $(SOURCES) -lSDL2 -lSDL2_ttf -o $(OUT_APP)

$(BUILD_LIB_heatshrink):
	make -C lib/heatshrink libraries

#$(OUT_LIB): $(OBJECTS)
#	ar rcs $(OUT_LIB) $(OBJECTS)

#$(LIBS)%.o: $(LIBS)%.c
#	$(CC_PREFIX) -I$(LIBS) -I$(HELPERS) -g $< -o $@
#$(HELPERS)%.o: $(HELPERS)%.c
#	$(CC_PREFIX) -I$(HELPERS) -g $< -o $@

clean:
	rm -rf $(OUT_DIR) $(APP)