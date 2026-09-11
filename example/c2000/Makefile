# Use bash with pipefail: recipes pipe the compiler through sed (COMPILER_COLOR),
# without this a compilation error would not stop make.
SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

# Disable default rules
.SUFFIXES:

# Toolchain and SDK
DEVICE=f2838x
TOOLCHAINPATH ?= /opt/ti/arm

C2000WAREPATH ?= /mnt/c/ti/C2000/C2000Ware_26_01_00_00
# Flash API (headers + .lib): shipped with C2000Ware, absent from its GitHub mirror
FLASHAPIPATH ?= $(C2000WAREPATH)/libraries/flash_api/$(DEVICE)
DRIVERLIBPATH=$(C2000WAREPATH)/driverlib/$(DEVICE)/driverlib_cm

# TI is using a different naming convention for the device libraries
DEVICE_UC = $(shell echo $(DEVICE) | sed 's/^./\U&/')

DEFINES=DEBUG _FLASH

LIBS=$(DEVICE_UC)_CM_FlashAPI.lib

LIBS_INCLUDES=\
	$(FLASHAPIPATH)/cm/lib \
	$(TOOLCHAINPATH)/lib \
	$(TOOLCHAINPATH)/include

CMDS=2838x_FLASH_lnk_cm.cmd

SRCS=$(shell find . -name '*.cpp' -o -name '*.c' -o -name '*.asm')

# Common sources all cores
SRCS += $(shell find $(realpath ../shared) -name '*.cpp')
SRCS += $(shell find $(realpath ../shared) -name '*.c')

# CanOpen Stack
SRCS += $(shell find $(realpath ../canopen-stack/src) -name '*.cpp')

# Device support: CM_init / __error__ (linked resource in the CCS project)
SRCS += $(C2000WAREPATH)/device_support/$(DEVICE)/common/source/cm.c

# Driverlib sources
SRCS+= $(DRIVERLIBPATH)/mcan.c \
	   $(DRIVERLIBPATH)/ipc.c \
	   $(DRIVERLIBPATH)/flash.c \
	   $(DRIVERLIBPATH)/sysctl.c 

SRCS+=$(DRIVERLIBSRCS)

INCLUDES= . ./device \
	$(TOOLCHAINPATH)/include \
	$(TOOLCHAINPATH)/include/c \
	$(DRIVERLIBPATH) \
	$(DRIVERLIBPATH)/inc \
	$(FLASHAPIPATH)/cm/include/FlashAPI \
	$(FLASHAPIPATH)/cm/include/FlashAPI/Constants \
	$(C2000WAREPATH)/device_support/$(DEVICE)/common/include \
	../canopen-stack/src \
	../canopen-stack/src/objects \
	../shared


DIRS = $(sort $(dir $(OBJS)))

OBJS=$(patsubst %.cpp, build/%.o, $(patsubst %.c, build/%.o, $(patsubst %.asm, build/%.o, $(SRCS))))

# LIBS = $(C2000WAREPATH)/libraries/flash_api/f2838x/cm/lib/F2838x_CM_FlashAPI.lib

ifeq ($(TOOLCHAINPATH),/opt/ti/clang)
CXX=$(TOOLCHAINPATH)/bin/tiararmclmclang
HEX=$(TOOLCHAINPATH)/bin/tiararmhexmclang
INCLUDES_FLAGS := $(foreach inc,$(INCLUDES),-I"$(inc)")
DEFINES_FLAGS := $(foreach def,$(DEFINES),-D$(def))
LIBS_INCLUDES := $(foreach lib,$(LIBS_INCLUDES),-Wl,-i$(lib))
LIBS=foreach lib,$(LIBS),-Wl,-l$(lib)

FLAGS=-march=thumbv7em -mcpu=cortex-m4 \
	-mfloat-abi=soft -mlittle-endian -mthumb -Oz -flto \
	-gdwarf-3  \
	-Werror=ti-pragmas -Werror=ti-macros -Werror=ti-intrinsics \
	-fno-short-wchar -fcommon -ffunction-sections \
	-include $(TOOLCHAINPATH)/include/c/ti_compatibility.h \
	$(DEFINES_FLAGS)

CXXFLAGS=$(FLAGS) -std=gnu++14

CFLAGS=$(FLAGS) -std=c11

CLFLAGS = $(FLAGS) \
	-Wl,-m"link.map" -Wl,--heap_size=2048 \
	-Wl,--stack_size=2048 \
	-Wl,--reread_libs -Wl, \ #--define=RAM
	-Wl,--diag_wrap=off -Wl,--display_error_number \
	-Wl,--verbose_diagnostics -Wl,--warn_sections \
	-Wl,--xml_link_info="link.xml" \
	-Wl,--rom_model \
	$(LIBS_INCLUDES)

ifeq ($(DEBUG),1)
	FLAGS += -g -O0
else
	FLAGS += -O2 --opt_for_speed=0
endif

OD = -o

PREPROC=-MMD -MP -MF"$(basename $(@)).d" -MT"$@"
else
CXX=$(TOOLCHAINPATH)/bin/armcl
HEX=$(TOOLCHAINPATH)/bin/armhex
INCLUDES_FLAGS := $(foreach inc,$(INCLUDES),--include_path="$(inc)")
DEFINES_FLAGS := $(foreach def,$(DEFINES),--define=$(def))
LIBS_INCLUDES := $(foreach lib,$(LIBS_INCLUDES),-I$(lib))
LIBS:=$(foreach lib,$(LIBS),-l$(lib))

FLAGS=-mv7M4 --code_state=16 --float_support=none -me \
	--parallel=8 \
	--diag_warning=225 \
	--diag_wrap=off --display_error_number \
	--gen_func_subsections=on \
	--c99 \
	$(DEFINES_FLAGS)

CXXFLAGS=$(FLAGS) --c++14

CFLAGS=$(FLAGS)

CLFLAGS = $(FLAGS) \
	-z \
	--heap_size=0 --stack_size=2048 \
	--warn_sections \
	--reread_libs \
	--rom_model \
	--xml_link_info="build/linkInfo.xml" \
	-mbuild/link.map \
	$(LIBS_INCLUDES)

OD = --output_file=

PREPROC=--preproc_with_compile --preproc_dependency="$(basename $@).d"

endif


GREEN = \033[0;32m
YELLOW = \033[0;33m
PINK = \033[0;35m
RED = \033[0;31m
NC = \033[0m

COMPILER_COLOR = 2>&1 | sed -e 's/\([Ee]rror[ :]\)/\x1b[0;31m\1\x1b[0m/g' -e 's/\([Ww]arning[ :]\)/\x1b[0;33m\1\x1b[0m/g'

TARGET = build/firmware.out

all: $(TARGET)
	$(HEX) --boot --gpio8 --ascii -o "build/firmware.hex" $(TARGET)

# all: $(OBJS) $(CMDS)
# 	$(call verbose, $(CXX) $(CLFLAGS) -o $(TARGET) $^ $(LIBS) ${COMPILER_COLOR}, echo "${YELLOW}LINKINK ${TARGET} ${NC}")

$(TARGET): $(OBJS) $(CMDS)
	$(call verbose, $(CXX) $(CLFLAGS) -o $@ $^ $(LIBS) ${COMPILER_COLOR}, echo "${YELLOW}LINKING $@...${NC}")

-include $(OBJS:.o=.d)

verbose = $(if $(VERBOSE),$(1),@$(1) && $(2))

build/%.o: %.cpp | builddir
	$(call verbose, $(CXX) -c $(CXXFLAGS) $(INCLUDES_FLAGS) $(PREPROC) $(OD)$@ $< ${COMPILER_COLOR}, echo "${GREEN}CCX $<${NC}") 

build/%.o: %.c | builddir
	$(call verbose, $(CXX) -c $(CFLAGS) $(INCLUDES_FLAGS) $(PREPROC) $(OD)$@ $< ${COMPILER_COLOR}, echo "${GREEN}CC $<${NC}")

build/%.o: %.asm | builddir
	$(CXX) -c $(CFLAGS) $(INCLUDES_FLAGS) $(PREPROC) $(OD)$@ $< $(COMPILER_COLOR)

builddir:
	mkdir -p $(DIRS)

clean:
	rm -rf build
	rm -f firmware.out

.PHONY: clean builddir all