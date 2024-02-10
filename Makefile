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

OPTFLAGS := -O4,p
INLINEFLAGS := -inline auto
CFLAGS := $(OPTFLAGS) $(INLINEFLAGS) -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn pragmas -pragma 'cats off'

ASFLAGS = -mgekko -I src -I include

######################## Targets #############################

$(foreach dir,$(SRC_DIRS) $(ASM_DIRS) $(DATA_DIRS),$(shell mkdir -p build/release/$(dir)))
$(foreach dir,$(SRC_DIRS) $(ASM_DIRS) $(DATA_DIRS),$(shell mkdir -p build/debug/$(dir)))

######################## Build #############################

A_FILES := $(foreach dir,$(BASEROM_DIR),$(wildcard $(dir)/*.a)) 

default: all

all: $(DTK) amcnotstub.a amcnotstubD.a

extract: $(DTK)
	$(info Extracting files...)
	@$(DTK) ar extract baserom/*.a --out baserom/out
	find baserom -name '*.o' | while read i; do \
		$(DTK) elf disasm $$i $${i%.o}.s ; \
	done

# clean extraction so extraction can be done again.
distclean:
	rm -rf $(BASEROM_DIR)/out
	rm -rf tools/dtk
	make clean

clean:
	rm -rf $(BUILD_DIR)
	rm -rf *.a

$(DTK): tools/dtk_version
	@echo "Downloading $@"
	$(QUIET) $(PYTHON) tools/download_dtk.py $< $@

build/debug/src/%.o: src/%.c
	$(CC) -c -opt level=0 -inline off $(CFLAGS) $< -o $@

build/release/src/%.o: src/%.c
	$(CC) -c -O4,p -inline auto $(CFLAGS) $< -o $@

################################ Build AR Files ###############################

amcnotstub_c_files := $(foreach dir,src/amcnotstub,$(wildcard $(dir)/*.c))
amcnotstub_o_files := $(foreach file,$(amcnotstub_c_files),$(BUILD_DIR)/release/$(file:.c=.o))
amcnotstub.a: $(amcnotstub_o_files)
	$(AR) -v -q $@ $?

amcnotstubD_c_files := $(foreach dir,src/amcnotstub,$(wildcard $(dir)/*.c))
amcnotstubD_o_files := $(foreach file,$(amcnotstubD_c_files),$(BUILD_DIR)/debug/$(file:.c=.o))
amcnotstubD.a: $(amcnotstubD_o_files)
	$(AR) -v -q $@ $?

# ------------------------------------------------------------------------------

.PHONY: asset_files o_files all clean distclean default split setup

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true

-include $(DEP_FILES)
