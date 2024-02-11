#################################################################
#              Dolphin SDK 2001 Libraries Makefile              #
#################################################################

ifneq (,$(findstring Windows,$(OS)))
  EXE := .exe
else
  WINE ?= 
endif

# If 0, tells the console to chill out. (Quiets the make process.)
VERBOSE ?= 0

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  HOST_OS := linux
else ifeq ($(UNAME_S),Darwin)
  HOST_OS := macos
else
  $(error Unsupported host/building OS <$(UNAME_S)>)
endif

BUILD_DIR := build
BASEROM_DIR := baserom
TARGET_LIBS := G2D              \
               ai               \
               amcnotstub       \
               amcstubs         \
               ar               \
               ax               \
               axfx             \
               base             \
               card             \
               db               \
               demo             \
               dolformat        \
               dsp              \
               dtk              \
               dvd              \
               fileCache        \
               gx               \
               hio              \
               mcc              \
               mix              \
               mtx              \
               odemustubs       \
               odenotstub       \
               os               \
               pad              \
               perf             \
               seq              \
               support          \
               syn              \
               texPalette       \
               vi

ifeq ($(VERBOSE),0)
  QUIET := @
endif

PYTHON := python3

# Every file has a debug version. Append D to the list.
TARGET_LIBS_DEBUG := $(addsuffix D,$(TARGET_LIBS))

# TODO, decompile
SRC_DIRS := $(shell find src -type d)

###################### Other Tools ######################

C_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
S_FILES := $(foreach dir,$(SRC_DIRS) $(ASM_DIRS),$(wildcard $(dir)/*.s))
DATA_FILES := $(foreach dir,$(DATA_DIRS),$(wildcard $(dir)/*.bin))
BASEROM_FILES := $(foreach dir,$(BASEROM_DIRS)\,$(wildcard $(dir)/*.s))

# Object files
O_FILES := $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file:.c=.c.o)) \
           $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file:.s=.s.o)) \
           $(foreach file,$(DATA_FILES),$(BUILD_DIR)/$(file:.bin=.bin.o)) \

DEP_FILES := $(O_FILES:.o=.d) $(DECOMP_C_OBJS:.o=.asmproc.d)

##################### Compiler Options #######################
findcmd = $(shell type $(1) >/dev/null 2>/dev/null; echo $$?)

# todo, please, better CROSS than this.
CROSS := powerpc-linux-gnu-

COMPILER_VERSION ?= 1.2.5

COMPILER_DIR := mwcc_compiler/GC/$(COMPILER_VERSION)
AS = $(CROSS)as
MWCC    := $(WINE) $(COMPILER_DIR)/mwcceppc.exe
AR = $(CROSS)ar
LD = $(CROSS)ld
OBJDUMP = $(CROSS)objdump
OBJCOPY = $(CROSS)objcopy
ifeq ($(HOST_OS),macos)
  CPP := clang -E -P -x c
else
  CPP := cpp
endif
DTK     := tools/dtk

CC        = $(MWCC)

######################## Flags #############################

CFLAGS := -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn pragmas -pragma 'cats off'
INCLUDES := -Isrc -Iinclude

ASFLAGS = -mgekko -I src -I include

######################## Targets #############################

$(foreach dir,$(SRC_DIRS) $(ASM_DIRS) $(DATA_DIRS),$(shell mkdir -p build/release/$(dir)))
$(foreach dir,$(SRC_DIRS) $(ASM_DIRS) $(DATA_DIRS),$(shell mkdir -p build/debug/$(dir)))

######################## Build #############################

A_FILES := $(foreach dir,$(BASEROM_DIR),$(wildcard $(dir)/*.a)) 

TARGET_LIBS := $(addprefix baserom/,$(addsuffix .a,$(TARGET_LIBS)))
TARGET_LIBS_DEBUG := $(addprefix baserom/,$(addsuffix .a,$(TARGET_LIBS_DEBUG)))

default: all

all: $(DTK) amcnotstub.a amcnotstubD.a amcstubs.a amcstubsD.a odemustubs.a odemustubsD.a odenotstub.a odenotstubD.a os.a osD.a

extract: $(DTK)
	$(info Extracting files...)
	@$(DTK) ar extract $(TARGET_LIBS) --out baserom/release/src
	@$(DTK) ar extract $(TARGET_LIBS_DEBUG) --out baserom/debug/src
    # Thank you GPT, very cool. Temporary hack to remove D off of inner src folders to let objdiff work.
	@for dir in $$(find baserom/debug/src -type d -name 'src'); do \
		find "$$dir" -mindepth 1 -maxdepth 1 -type d | while read subdir; do \
			mv "$$subdir" "$${subdir%?}"; \
		done \
	done
	# Disassemble the objects and extract their dwarf info.
	find baserom -name '*.o' | while read i; do \
		$(DTK) elf disasm $$i $${i%.o}.s ; \
		$(DTK) dwarf dump $$i -o $${i%.o}_DWARF.c ; \
	done

# clean extraction so extraction can be done again.
distclean:
	rm -rf $(BASEROM_DIR)/release
	rm -rf $(BASEROM_DIR)/debug
	rm -rf tools/dtk
	make clean

clean:
	rm -rf $(BUILD_DIR)
	rm -rf *.a

$(DTK): tools/dtk_version
	@echo "Downloading $@"
	$(QUIET) $(PYTHON) tools/download_dtk.py $< $@

build/debug/src/%.o: src/%.c
	$(CC) -c -opt level=0 -inline off -schedule off -sym on $(CFLAGS) -I- $(INCLUDES) -DDEBUG $< -o $@

build/release/src/%.o: src/%.c
	$(CC) -c -O4,p -inline auto $(CFLAGS) -I- $(INCLUDES) -DRELEASE $< -o $@

################################ Build AR Files ###############################

amcnotstub_c_files := $(foreach dir,src/amcnotstub,$(wildcard $(dir)/*.c))
amcnotstub_o_files := $(foreach file,$(amcnotstub_c_files),$(BUILD_DIR)/release/$(file:.c=.o))
amcnotstub.a: $(amcnotstub_o_files)
	$(AR) -v -q $@ $?

amcnotstubD_c_files := $(foreach dir,src/amcnotstub,$(wildcard $(dir)/*.c))
amcnotstubD_o_files := $(foreach file,$(amcnotstubD_c_files),$(BUILD_DIR)/debug/$(file:.c=.o))
amcnotstubD.a: $(amcnotstubD_o_files)
	$(AR) -v -q $@ $?

amcstubs_c_files := $(foreach dir,src/amcstubs,$(wildcard $(dir)/*.c))
amcstubs_o_files := $(foreach file,$(amcstubs_c_files),$(BUILD_DIR)/release/$(file:.c=.o))
amcstubs.a: $(amcstubs_o_files)
	$(AR) -v -q $@ $?

amcstubsD_c_files := $(foreach dir,src/amcstubs,$(wildcard $(dir)/*.c))
amcstubsD_o_files := $(foreach file,$(amcstubsD_c_files),$(BUILD_DIR)/debug/$(file:.c=.o))
amcstubsD.a: $(amcstubsD_o_files)
	$(AR) -v -q $@ $?

odemustubs_c_files := $(foreach dir,src/odemustubs,$(wildcard $(dir)/*.c))
odemustubs_o_files := $(foreach file,$(odemustubs_c_files),$(BUILD_DIR)/release/$(file:.c=.o))
odemustubs.a: $(odemustubs_o_files)
	$(AR) -v -q $@ $?

odemustubsD_c_files := $(foreach dir,src/odemustubs,$(wildcard $(dir)/*.c))
odemustubsD_o_files := $(foreach file,$(odemustubsD_c_files),$(BUILD_DIR)/debug/$(file:.c=.o))
odemustubsD.a: $(odemustubsD_o_files)
	$(AR) -v -q $@ $?

odenotstub_c_files := $(foreach dir,src/odenotstub,$(wildcard $(dir)/*.c))
odenotstub_o_files := $(foreach file,$(odenotstub_c_files),$(BUILD_DIR)/release/$(file:.c=.o))
odenotstub.a: $(odenotstub_o_files)
	$(AR) -v -q $@ $?

odenotstubD_c_files := $(foreach dir,src/odenotstub,$(wildcard $(dir)/*.c))
odenotstubD_o_files := $(foreach file,$(odenotstubD_c_files),$(BUILD_DIR)/debug/$(file:.c=.o))
odenotstubD.a: $(odenotstubD_o_files)
	$(AR) -v -q $@ $?

os_c_files := $(foreach dir,src/os,$(wildcard $(dir)/*.c))
os_o_files := $(foreach file,$(os_c_files),$(BUILD_DIR)/release/$(file:.c=.o))
os.a: $(os_o_files)
	$(AR) -v -q $@ $?

osD_c_files := $(foreach dir,src/os,$(wildcard $(dir)/*.c))
osD_o_files := $(foreach file,$(osD_c_files),$(BUILD_DIR)/debug/$(file:.c=.o))
osD.a: $(osD_o_files)
	$(AR) -v -q $@ $?

# ------------------------------------------------------------------------------

.PHONY: asset_files o_files all clean distclean default split setup

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true

-include $(DEP_FILES)
