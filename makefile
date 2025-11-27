# ============== MCU Configuration ============== #
# Change these for your specific microcontroller  #

# MCU name
MCU      ?= atmega64a
# MCU frequency (16 MHz)
F_CPU    ?= 16000000UL
# bit clock period (µs, float)
BITCLOCK ?= 11.0

# ============= Programmer Settings ============= #

PROGRAMMER       ?= usbasp-clone
PROGRAMMER_ARGS   = -B $(BITCLOCK)

# =================== Targets =================== #
# Target will be built as <target>.hex and it's   #
# entry point is in <target>/<target>.c           #

TARGETS := \
blink \
serial

# =================== Folders =================== #

COMMON_DIR = common
BUILD_DIR  = build
HEX_DIR    = programs

# ============= Programs & Arguments ============ #

CC         = avr-gcc
OBJCOPY    = avr-objcopy
AVRDUDE    = avrdude

CFLAGS     = -mmcu=$(MCU) -Os -DF_CPU=$(F_CPU) -Wall -Wextra -pedantic -MMD
LDFLAGS    = -mmcu=$(MCU) -Wl,--gc-sections

# =============================================== #
# No need to edit below this line for normal use  #

# Shared source files (compiled once)
COMMON_SRC := $(wildcard $(COMMON_DIR)/*.c)
COMMON_OBJ := $(COMMON_SRC:$(COMMON_DIR)/%.c=$(BUILD_DIR)/common/%.o)
COMMON_DEP := $(COMMON_OBJ:.o=.d)

# Per-program sources/objects. make sure file containing main is first
define PROGRAM_template
  $(1)_SRC := $(1)/$(1).c $$(filter-out $(1)/$(1).c,$$(wildcard $(1)/*.c))
  $(1)_OBJ := $$($(1)_SRC:$(1)/%.c=$(BUILD_DIR)/%.o)
  $(1)_DEP := $$($(1)_OBJ:.o=.d)
endef

$(foreach prog,$(TARGETS),$(eval $(call PROGRAM_template,$(prog))))

ALL_PROG_OBJ := $(foreach prog,$(TARGETS),$($(prog)_OBJ))
ALL_PROG_DEP := $(foreach prog,$(TARGETS),$($(prog)_DEP))

# Build paths
ELFFILES := $(addprefix $(BUILD_DIR)/,$(addsuffix .elf,$(TARGETS)))
HEXFILES := $(addprefix $(HEX_DIR)/,$(addsuffix .hex,$(TARGETS)))

all: $(HEXFILES)
	@echo ""
	@echo "Build complete!"
	@echo "Available programs: $(TARGETS)"
	@echo "Usage:"
	@echo "  make upload-<name>  Upload program to MCU"
	@echo "  make fuses          Program ATmega64A fuses"
	@echo "  make clean          Remove $(BUILD_DIR)/ folder"
	@echo "  make clean-all      Remove $(BUILD_DIR)/ and $(HEX_DIR)/ folders"
	@echo ""

# Dirs
$(BUILD_DIR) $(HEX_DIR):
	mkdir -p $@
	echo "*" > $@/.gitignore

# Link each program: its own objects + all common objects
define LINK_rule
$(BUILD_DIR)/$(1).elf: $$($(1)_OBJ) $(COMMON_OBJ) | $(BUILD_DIR)
	@echo "Linking $$@"
	$(CC) $(LDFLAGS) $$^ -o $$@
endef

$(foreach prog,$(TARGETS),$(eval $(call LINK_rule,$(prog))))

# Compile common files
$(BUILD_DIR)/%.o: $(COMMON_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile program-specific files
$(BUILD_DIR)/%.o: */%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# .elf -> .hex conversion
$(HEX_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(HEX_DIR) $(BUILD_DIR)
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

# Dependency tracking
-include $(COMMON_DEP) $(ALL_PROG_DEP)

# avrdude stuff
AVRDUDE_COMMAND = $(AVRDUDE) -c $(PROGRAMMER) $(PROGRAMMER_ARGS) -p $(MCU)

# Fuse settings for ATmega64A with external 16MHz crystal + SPIEN enabled
fuses:
	$(AVRDUDE_COMMAND) -U lfuse:w:0x9e:m -U hfuse:w:0xc1:m -U efuse:w:0xff:m

# Upload
FORCE: # empty rule is always newer than anything

define UPLOAD_RULE
upload-$(1): $(HEX_DIR)/$(1).hex FORCE
	@echo "=== Flashing $(1) ==="
	$(AVRDUDE_COMMAND) -U flash:w:$$<:i
	@echo "=== $(1) done ===="
endef

$(foreach prog,$(TARGETS),$(eval $(call UPLOAD_RULE,$(prog))))
.PHONY: $(addprefix upload-,$(TARGETS))

# remove intermediate build files
clean:
	rm -rf $(BUILD_DIR)

# remove all built files
clean-all:
	rm -rf $(BUILD_DIR) $(HEX_DIR)

.PHONY: all fuses clean clean-all
