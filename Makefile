ifeq ($(strip $(DEVKITPRO)),)
$(error Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro)
endif

include $(DEVKITARM)/base_tools

#-------------------------------------------------------------------------------

export BUILD := build
export SRC := src
export BINARY := $(BUILD)/linked.o
export ARMIPS := armips
export ROM_CODE := BPRE
export LD := $(PREFIX)ld
export PREPROC := ../../pokeruby/tools/preproc/preproc
export CHARMAP := ../../pokeruby/charmap.txt
export INCLUDE := ../headers/build/include
export ASFLAGS := -mthumb
export CFLAGS := -g -O2 -Wall -mthumb -std=c11 -I $(INCLUDE) -mcpu=arm7tdmi \
	-march=armv4t -mno-thumb-interwork -fno-inline -fno-builtin -mlong-calls -DROM_$(ROM_CODE)
export LDFLAGS := -T layout.ld -T ../headers/build/linker/$(ROM_CODE).ld -r

#-------------------------------------------------------------------------------

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Sources
C_SRC=$(call rwildcard,$(SRC),*.c)
S_SRC=$(call rwildcard,$(SRC),*.s)

# Binaries
C_OBJ=$(C_SRC:%=%.o)
S_OBJ=$(S_SRC:%=%.o)
OBJECTS=$(C_OBJ) $(S_OBJ)

#-------------------------------------------------------------------------------

.PHONY: all clean test

all: main.s $(BINARY) $(call rwildcard,patches,*.s)
	$(ARMIPS) main.s

clean:
	rm -rf build

test:
	$(MAKE) -f test/Makefile

$(BINARY): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(addprefix $(BUILD)/,$^)

%.c.o: %.c $(call rwildcard,$(SRC),*.h)
	@mkdir -p $(BUILD)/$(@D)
	$(CC) $(CFLAGS) -c $< -o $(BUILD)/$@

%.s.o: %.s
	@mkdir -p $(BUILD)/$(@D)
	$(PREPROC) $< $(CHARMAP) | $(AS) $(ASFLAGS) -o $(BUILD)/$@
