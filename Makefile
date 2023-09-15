LIBS = flipper/
HELPERS = helpers/
APP = flippulator_app_copy/
SOURCES = $(shell find $(LIBS) -name "*.c") $(shell find $(HELPERS) -name "*.c")
SRC_APP = $(shell find $(APP) -name "*.c")
#OBJECTS = ${subst .c,.o,$(SOURCES)}
OUT_APP = out
#OUT_LIB_NAME = flipper
#OUT_LIB = lib$(OUT_LIB_NAME).a
#CC_PREFIX = gcc -c -Wall# -fPIC
CC_PREFIX_FINAL = gcc -Wall -Wno-main
#CC_PREFIX = clang -c

all: $(OUT_APP)

$(OUT_APP):# $(OUT_LIB)# 
	$(CC_PREFIX_FINAL) -I$(LIBS) -I$(HELPERS) $(SRC_APP) $(SOURCES) -lncurses -o $(OUT_APP)

#$(OUT_LIB): $(OBJECTS)
#	ar rcs $(OUT_LIB) $(OBJECTS)

#$(LIBS)%.o: $(LIBS)%.c
#	$(CC_PREFIX) -I$(LIBS) -I$(HELPERS) -g $< -o $@
#$(HELPERS)%.o: $(HELPERS)%.c
#	$(CC_PREFIX) -I$(HELPERS) -g $< -o $@

clean:
	rm -rf $(OUT_APP) $(APP)