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
export AIF := deps/pokeruby/tools/aif2pcm/aif2pcm
export BIN2C := deps/pokeruby/tools/bin2c/bin2c
export PREPROC := deps/pokeruby/tools/preproc/preproc
export CHARMAP := charmap.txt
export INCLUDE := -I deps/pokeagb/build/include -I $(SRC) -I .
export ASFLAGS := -mthumb
export CFLAGS := -g -O2 -Wall -mthumb -std=c11 $(INCLUDE) -mcpu=arm7tdmi \
	-march=armv4t -mno-thumb-interwork -fno-inline -fno-builtin -mlong-calls -DROM_$(ROM_CODE) \
	-fdiagnostics-color
export LDFLAGS := -T layout.ld -T deps/pokeagb/build/linker/$(ROM_CODE).ld -r
export DEPDIR = .d
export DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

#-------------------------------------------------------------------------------

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Generated
IMAGES=$(call rwildcard,images,*.png)

# Sources
C_SRC=$(call rwildcard,$(SRC),*.c)
S_SRC=$(call rwildcard,$(SRC),*.s)
GEN_SRC=$(IMAGES:images/%.png=generated/images/%.c)

# Binaries
C_OBJ=$(C_SRC:%=%.o)
S_OBJ=$(S_SRC:%=%.o)
GEN_OBJ=$(GEN_SRC:%=%.o)
OBJECTS=$(addprefix $(BUILD)/,$(GEN_OBJ) $(C_OBJ) $(S_OBJ))

#-------------------------------------------------------------------------------

.PHONY: all clean test generated images patch pokedex pokedex_generate

all: pokedex main.s $(BINARY) $(call rwildcard,patches,*.s)
	sh battle_backgrounds
	@echo -e "\e[1;32mCreating ROM...\e[0m"

	@echo -e "\n\e[1mSpace Usage:"
	@$(PREFIX)size $(BINARY) | \
		awk 'FNR==2 {printf "\t%d B\n\t%.2f KiB\n\t%.2f MiB\n",$$4,$$4/1024,$$4/1024/1024;}'
	@echo -e "\e[21m"

	@$(ARMIPS) main.s

patch: all
	deps/patch/patch ups roms/BPRE0.gba build/multi.gba build/patch.ups
	deps/patch/patch ppf roms/BPRE0.gba build/multi.gba build/patch.ppf

clean:
	rm -rf build
	rm -rf generated

test:
	$(MAKE) -f test/Makefile

$(BINARY): $(OBJECTS) $(BUILD)/pokedex/pokedex.o
	@echo -e "\e[1;32mLinking ELF binary $@\e[0m"
	@$(LD) $(LDFLAGS) -o $@ $^

$(BUILD)/%.c.o: %.c $(DEPDIR)/%.d
	@echo -e "\e[32mCompiling $<\e[0m"
	@mkdir -p $(@D)
	@mkdir -p $(DEPDIR)/$<
	@$(CC) $(DEPFLAGS) $(CFLAGS) -E -c $< -o $*.i
	@$(PREPROC) $*.i $(CHARMAP) | $(CC) $(CFLAGS) -x c -o $@ -c -
	@mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

$(BUILD)/%.s.o: %.s
	@echo -e "\e[32mAssembling $<\e[0m"
	@mkdir -p $(@D)
	@$(PREPROC) $< $(CHARMAP) | $(AS) $(ASFLAGS) -o $@

generated/images/%.c: images/%.png images/%.grit
	@echo -e "\e[34mProcessing image $<\e[0m"
	@mkdir -p $(@D)
	@grit $< -o $@ -ff$(<:%.png=%.grit)
	@python scripts/grithack.py $@

#-------------------------------------------------------------------------------
# Pokedex
#-------------------------------------------------------------------------------

POKEDEX_CRIES = $(wildcard pokedex/assets/*_cry.aif)

POKEDEX_SPRITES = $(wildcard pokedex/assets/*_sprite.png)

POKEDEX_ICONS = $(wildcard pokedex/assets/*_icon.png)

POKEDEX_TABLE_SRC = pokedex/tables/sprites pokedex/tables/names pokedex/tables/base_stats \
	pokedex/tables/icons pokedex/tables/learnset

POKEDEX_ASSETS = $(POKEDEX_CRIES:%=$(BUILD)/generated/%.c.o) \
	$(POKEDEX_ICONS:%=$(BUILD)/generated/%.c.o) \
	$(POKEDEX_SPRITES:%=$(BUILD)/generated/%.c.o)

POKEDEX_TABLES = $(POKEDEX_TABLE_SRC:%=$(BUILD)/generated/%.c.o)

pokedex: pokedex/pokedex.lock $(BUILD)/pokedex/pokedex.o

pokedex/pokedex.lock: pokedex/config.json
	@python scripts/pokedex/generate.py pokedex/config.json generated/pokedex

$(BUILD)/pokedex/pokedex.o: $(POKEDEX_ASSETS) $(POKEDEX_TABLES)
	@echo -e "\e[32m[Pokedex] Linking...\e[0m"
	@$(LD) -r -o $@ $^

generated/pokedex/%_cry.aif.c: generated/pokedex/%_cry.aif.bin
	@echo '#include <pokeagb/pokeagb.h>' > $@
	@$(BIN2C) $< pokedex_assets_$(*F)_cry >> $@

generated/pokedex/%_cry.aif.bin: pokedex/%_cry.aif
	@mkdir -p $(@D)
	@$(AIF) $< $@ --compress

generated/pokedex/%_icon.png.c: pokedex/%_icon.png
	@mkdir -p $(@D)
	@grit $< -o $@ -p! -gB4 -gu8 -fh! -spokedex_assets_$(*F)_icon

generated/pokedex/%_sprite.png.c: pokedex/%_sprite.png
	@mkdir -p $(@D)
	@bash scripts/pokedex/gensprite.sh $(BUILD) $(*F) $< $@

#-------------------------------------------------------------------------------

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d
-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(C_SRC)))
