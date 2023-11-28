LIBS = flipper/
LIBS_HAL = flipper_hal/
EXT_LIB_ALL = lib/
#EXT_LIB_u8g2 = lib/u8g2/
HELPERS = helpers/
APP = flippulator_app_copy/
SOURCES = $(shell find $(LIBS) -name "*.c") $(shell find $(LIBS_HAL) -name "*.c") $(shell find $(HELPERS) -name "*.c") $(shell find $(EXT_LIB_ALL) -name "*.c")
SRC_APP = $(shell find $(APP) -name "*.c")
OBJECTS = ${subst .c,.o,$(SOURCES)} ${subst .c,.o,$(SRC_APP)}
OUT_DIR = $(shell cat /tmp/flippulator_temp_out_app_dir)
OUT_APP_NAME = $(shell cat /tmp/flippulator_temp_out_app_name)
CC_EXTRA = $(shell cat /tmp/flippulator_temp_cc_extra)
OUT_APP = $(OUT_DIR)$(OUT_APP_NAME)
SRC_FONT = haxrcorp-4089.ttf
OUT_FONT = $(OUT_DIR)haxrcorp-4089.ttf
OUT_CR = $(OUT_DIR)copyright.txt
OUT_ALL = $(OUT_DIR) $(OUT_APP)
#OUT_LIB_NAME = flipper
#OUT_LIB = lib$(OUT_LIB_NAME).a
#CC_PREFIX = gcc -c -Wall# -fPIC
CC_PREFIX_FINAL = gcc -O2 -Wall -Wno-main -g -m32 -DFLIPPULATOR_APP_ID=\"$(OUT_APP_NAME)\" -D_FLIPPULATOR
CC_POSTFIX_FINAL =  -lSDL2 -lSDL2_ttf -lm -lbsd
BUILD_LIB_heatshrink_PATH = lib/heatshrink/
BUILD_LIB_heatshrink = $(BUILD_LIB_heatshrink_PATH)libheatshrink_static.a $(BUILD_LIB_heatshrink_PATH)libheatshrink_dynamic.a
#CC_PREFIX = clang -c
CR_TEXT = \
	"The FontStruction “HaxrCorp 4089” (https://fontstruct.com/fontstructions/show/192981) by “sahwar” is licensed under a Creative Commons Attribution Share Alike license (http://creativecommons.org/licenses/by-sa/3.0/)."
WASM_DIR = wasm/
TIMESTAMPS = timestamps.json
TIMESTAMPS_TMP = /tmp/flippulator_timestamps.json

all: $(OUT_ALL)

$(OUT_DIR): $(OUT_APP) $(SRC_FONT)
	cp $(SRC_FONT) $(OUT_FONT)
	echo $(CR_TEXT) > $(OUT_CR)

$(OUT_APP):
	mkdir $(OUT_DIR)
	touch $(TIMESTAMPS)
	if [ ! -s $(TIMESTAMPS) ]; then \
		echo '{}' > $(TIMESTAMPS); \
	fi
#	$(CC_PREFIX_FINAL) $(CC_EXTRA) -I$(LIBS) -I$(LIBS_HAL) -I$(HELPERS) -I$(EXT_LIB_ALL) $(SRC_APP) $(SOURCES) -lSDL2 -lSDL2_ttf -lm -lbsd -o $(OUT_APP)
	for file in $(SRC_APP) $(SOURCES); do \
		jsout=$$(node compile.js $$file); \
		objname=$$(echo $$jsout | jq -r '.obj'); \
		dateextr=$$(echo $$jsout | jq -r '.time'); \
		if [ $$objname ]; then \
			$(CC_PREFIX_FINAL) -c $(CC_EXTRA) -I$(LIBS) -I$(LIBS_HAL) -I$(HELPERS) -I$(EXT_LIB_ALL) "$$file" -o $$objname $(CC_POSTFIX_FINAL); \
			echo CC: $$file = $$objname; \
			jq '.["'$$file'"] = "'$$dateextr'"' $(TIMESTAMPS) > $(TIMESTAMPS_TMP); mv $(TIMESTAMPS_TMP) $(TIMESTAMPS); \
		fi; \
	done
	$(CC_PREFIX_FINAL) $(OBJECTS) -o $(OUT_APP) $(CC_POSTFIX_FINAL)

#$(BUILD_LIB_heatshrink):
#	make -C lib/heatshrink libraries

#$(OUT_LIB): $(OBJECTS)
#	ar rcs $(OUT_LIB) $(OBJECTS)

#$(LIBS)%.o: $(LIBS)%.c
#	$(CC_PREFIX) -I$(LIBS) -I$(HELPERS) -g $< -o $@
#$(HELPERS)%.o: $(HELPERS)%.c
#	$(CC_PREFIX) -I$(HELPERS) -g $< -o $@

clean:
	rm -rf $(OUT_DIR) $(APP) $(WASM_DIR)

$(WASM_DIR):
	mkdir $(WASM_DIR)

#wasm: $(WASM_DIR)
#	emcc -O2 $(SOURCES) $(SRC_APP) $(CC_EXTRA) -g -s WASM=1 -s USE_SDL=2 -s USE_SDL_TTF=2 --preload-file $(SRC_FONT) -o $(WASM_DIR)index.html -I$(LIBS) -I$(LIBS_HAL) -I$(EXT_LIB_ALL) -I$(HELPERS)

