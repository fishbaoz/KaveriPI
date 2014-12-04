#*****************************************************************************
#
# Copyright (c) 2013-2014, Sage Electronic Engineering, LLC
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of Advanced Micro Devices, Inc. nor the names of
#       its contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#*****************************************************************************

.NOTPARALLEL:

AGESA_ROOT ?= ./../../..
-include $(AGESA_ROOT)/.xcompile

# Set Toolchain Variables
ifneq (,$(CPP_i386))
CPP := $(CPP_i386)
endif

ifneq (,$(CC_i386))
CC := $(CC_i386)
endif

ifneq (,$(AS_i386))
AS := $(AS_i386)
endif

ifneq (,$(AR_i386))
AR := $(AR_i386)
endif

ifneq (,$(LD_i386))
LD := $(LD_i386)
endif

ifneq (,$(OBJCOPY_i386))
OBJCOPY := $(OBJCOPY_i386)
else
OBJCOPY ?= objcopy
endif

# Compiler flags...
CFLAGS ?= -pipe -march=barcelona -m32 -masm=att


# Include paths...
Debug_Include_Path ?=   -I"../../build/" -I"../../binaryPI" -I"../../" -I"../../Include/" -I"../../Proc/IDS/" -I"../../Lib/" -I"../../Proc/CPU/" -I"../../Proc/CPU/Family/" -I"../../Proc/CPU/Feature/" -I"../../Proc/Common/" -I"../../Proc/HT/" -I"../../Proc/Mem/" -I"../../Proc/IDS/Debug/" -I"../../Proc/Recovery/Mem/" -I"../../Proc/GNB/Common/" -I"../../Proc/GNB/Modules/GnbInitTN/" -I"../../Proc/GNB/Modules/GnbInitBK/" -I"../../Proc/GNB/Modules/GnbInitKV/" -I"../../Proc/GNB/Modules/GnbInitKB/" -I"../../Proc/GNB/Modules/GnbInitML/"
Release_Include_Path ?= -I"../../build/" -I"../../binaryPI" -I"../../" -I"../../Include/" -I"../../Proc/IDS/" -I"../../Lib/" -I"../../Proc/CPU/" -I"../../Proc/CPU/Family/" -I"../../Proc/CPU/Feature/" -I"../../Proc/Common/" -I"../../Proc/HT/" -I"../../Proc/Mem/" -I"../../Proc/IDS/Debug/" -I"../../Proc/Recovery/Mem/" -I"../../Proc/GNB/Common/" -I"../../Proc/GNB/Modules/GnbInitTN/" -I"../../Proc/GNB/Modules/GnbInitBK/" -I"../../Proc/GNB/Modules/GnbInitKV/" -I"../../Proc/GNB/Modules/GnbInitKB/" -I"../../Proc/GNB/Modules/GnbInitML/"

# Library paths...
Library_Name ?= B2_Image
Debug_Library_Path ?= $(AGESA_ROOT)/AGESA/build/gccDebug
Release_Library_Path ?= $(AGESA_ROOT)/AGESA/build/gccRelease

# Build paths...
Debug_Build_Path ?= $(AGESA_ROOT)/AGESA/build/Debug/$(Library_Name)
Release_Build_Path ?= $(AGESA_ROOT)/AGESA/build/Release/$(Library_Name)

# Additional libraries...
Debug_Libraries=--start-group $(foreach library,$(wildcard $(Debug_Library_Path)/lib*.a),-l$(patsubst $(Debug_Library_Path)/lib%.a,%,$(library))) --end-group
Release_Libraries=--start-group $(foreach library,$(wildcard $(Release_Library_Path)/lib*.a),-l$(patsubst $(Release_Library_Path)/lib%.a,%,$(library))) --end-group

# Preprocessor definitions...
Debug_Preprocessor_Definitions=-D B2_IMAGE -D GCC_BUILD -D DEBUG
Release_Preprocessor_Definitions=-D GCC_BUILD -D RELEASE

# Implictly linked object files...
Debug_Implicitly_Linked_Objects=
Release_Implicitly_Linked_Objects=

# Compiler flags...
Debug_Compiler_Flags=$(CFLAGS) -O0 -ggdb
Release_Compiler_Flags=$(CFLAGS) -Os

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug Release

objects_debug := $(patsubst %.S,$(Debug_Build_Path)/%.o,$(wildcard *.S)) $(patsubst %.c,$(Debug_Build_Path)/%.o,$(wildcard *.c))
objects_release := $(patsubst %.S,$(Release_Build_Path)/%.o,$(wildcard *.S)) $(patsubst %.c,$(Release_Build_Path)/%.o,$(wildcard *.c))
dependencies_debug := $(patsubst %.S,$(Debug_Build_Path)/%.d,$(wildcard *.S)) $(patsubst %.c,$(Debug_Build_Path)/%.d,$(wildcard *.c))
dependencies_release := $(patsubst %.S,$(Release_Build_Path)/%.d,$(wildcard *.S)) $(patsubst %.c,$(Release_Build_Path)/%.d,$(wildcard *.c))

# Builds the Debug configuration...
.PHONY: Debug
Debug: create_folders $(Debug_Build_Path)/hobTransfer.o
	$(LD) --verbose -T $(AGESA_ROOT)/AGESA/binaryPI/AGESA.ld                 \
		$(foreach path,$(Debug_Library_Path),-L"$(path)")                    \
		-Map=$(Debug_Library_Path)/AGESA_B2_$(@F).map                        \
		-nostdlib -nostartfiles                                              \
		-u mImageID                                                          \
		-soname=libAgesa_B2_Target.so                                        \
		-o $(Debug_Library_Path)/libAgesa_B2_Target.so                       \
		$(Debug_Build_Path)/hobTransfer.o                                    \
		$(Debug_Implicitly_Linked_Objects)                                   \
		$(Debug_Libraries)
	$(OBJCOPY) -O binary $(Debug_Library_Path)/libAgesa_B2_Target.so $(Debug_Library_Path)/AGESA.bin

-include $(dependencies_debug)
$(Debug_Build_Path)/%.o: %.c
	@printf "\tCC\t[debug] $(<F)\n"; \
	$(CC) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -pipe -c $< $(INCLUDE) -o $@ -MMD
	$(CC) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -pipe -E $< $(INCLUDE) -dM  | sort > $(Debug_Build_Path)/source/$(<F)

$(Debug_Build_Path)/%.o: %.S
	@printf "\tCC\t[debug] $(<F)\n"; \
	$(CC) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -pipe -x assembler -masm=att -c $< $(INCLUDE) -o $@ -MMD
	$(CC) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -pipe -x assembler -masm=att -E $< $(INCLUDE) -dM | sort > $(Debug_Build_Path)/source/$(<F)

# Builds the Release configuration...
.PHONY: Release
Release: create_folders $(Release_Build_Path)/hobTransfer.o
	$(LD) --verbose -T $(AGESA_ROOT)/AGESA/binaryPI/AGESA.ld                 \
		$(foreach path,$(Release_Library_Path),-L"$(path)")                  \
		-Map=$(Release_Library_Path)/AGESA_B2_$(@F).map                      \
		-nostdlib -nostartfiles --strip-all                                  \
		-u mImageID                                                          \
		-soname=libAgesa_B2_Target.so                                        \
		-o $(Release_Library_Path)/libAgesa_B2_Target.so                     \
		$(Release_Build_Path)/hobTransfer.o                                  \
		$(Release_Implicitly_Linked_Objects)                                 \
		$(Release_Libraries)
	$(OBJCOPY) -O binary $(Release_Library_Path)/libAgesa_B2_Target.so $(Release_Library_Path)/AGESA.bin

-include $(dependencies_release)
$(Release_Build_Path)/%.o: %.c
	@printf "\tCC\t[release] $(<F)\n"; \
	$(CC) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -pipe -c $< $(INCLUDE) -o $@ -MMD

$(Release_Build_Path)/%.o: %.S
	@printf "\tCC\t[release] $(<F)\n"; \
	$(CC) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -pipe -x assembler -masm=att -c $< $(INCLUDE) -o $@ -MMD

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p $(Debug_Build_Path)/source                 \
	         $(Debug_Library_Path)                      \
	         $(Release_Build_Path)/source               \
	         $(Release_Library_Path)

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -rf $(Debug_Build_Path)                           \
	       $(Debug_Library_Path)/lib$(Library_Name).*    \
	       $(Debug_Library_Path)/libAgesa_B2_Target.*    \
	       $(Debug_Library_Path)/AGESA.bin               \
	       $(Release_Build_Path)                         \
	       $(Release_Library_Path)/lib$(Library_Name).*  \
	       $(Release_Library_Path)/libAgesa_B2_Target.*  \
	       $(Release_Library_Path)/AGESA.bin

