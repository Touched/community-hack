ifeq ($(strip $(DEVKITPRO)),)
$(error Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro)
endif

include $(DEVKITARM)/base_tools

#-------------------------------------------------------------------------------

export BUILD := build
export SRC := src
export BINARY := $(BUILD)/linked.o
export ARMIPS ?= armips
export ROM_CODE := BPRE
export LD := $(PREFIX)ld
export PREPROC := deps/pokeruby/tools/preproc/preproc
export CHARMAP := charmap.txt
export INCLUDE := -I deps/pokeagb/build/include -I $(SRC)
export ASFLAGS := -mthumb
export CFLAGS := -g -O2 -Wall -mthumb -std=c11 $(INCLUDE) -mcpu=arm7tdmi \
	-march=armv4t -mno-thumb-interwork -fno-inline -fno-builtin -mlong-calls -DROM_$(ROM_CODE)
export LDFLAGS := -T layout.ld -T deps/pokeagb/build/linker/$(ROM_CODE).ld -r

#-------------------------------------------------------------------------------

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Sources
C_SRC=$(call rwildcard,$(SRC),*.c)
S_SRC=$(call rwildcard,$(SRC),*.s)

# Binaries
C_OBJ=$(C_SRC:%=%.o)
S_OBJ=$(S_SRC:%=%.o)
OBJECTS=$(addprefix $(BUILD)/,$(C_OBJ) $(S_OBJ))

# Generated
IMAGES=$(call rwildcard,images,*.png)

#-------------------------------------------------------------------------------

.PHONY: all clean test generated images patch

all: generated main.s $(BINARY) $(call rwildcard,patches,*.s)
	$(ARMIPS) main.s

patch: all
	deps/patch/patch roms/BPRE0.gba build/multi.gba build/patch.ups

clean:
	rm -rf build
	rm -rf $(SRC)/generated

test:
	$(MAKE) -f test/Makefile

generated: images
images: $(IMAGES:images/%.png=$(SRC)/generated/images/%.c)

$(BINARY): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD)/%.c.o: %.c $(call rwildcard,$(SRC),*.h)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -E -c $< -o $*.i
	$(PREPROC) $*.i $(CHARMAP) | $(CC) $(CFLAGS) -x c -o $@ -c -

$(BUILD)/%.s.o: %.s
	@mkdir -p $(@D)
	$(PREPROC) $< $(CHARMAP) | $(AS) $(ASFLAGS) -o $@

$(SRC)/generated/images/%.c: images/%.png images/%.grit
	@mkdir -p $(@D)
	grit $< -o $@ -ff$(<:%.png=%.grit)
	python scripts/grithack.py $@
	