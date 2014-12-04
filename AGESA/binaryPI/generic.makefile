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

.NOTPARALLEL: create_folders

AGESA_ROOT ?= $(PWD)/..
SOURCE_DIRECTORY ?= $(CURDIR)
RECURSIVE ?= y

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

# Compiler flags...
CFLAGS ?= -pipe -march=barcelona -m32 -masm=att

# Include paths...
INCLUDE ?= -I"."                                                      \
           -I"$(AGESA_ROOT)/AGESA/build/"                             \
           -I"$(AGESA_ROOT)/AGESA/"                                   \
           -I"$(AGESA_ROOT)/AGESA/Lib/"                               \
           -I"$(AGESA_ROOT)/AGESA/Include/"                           \
           -I"$(AGESA_ROOT)/AGESA/Proc/IDS/"                          \
           -I"$(AGESA_ROOT)/AGESA/Proc/IDS/Family/"                   \
           -I"$(AGESA_ROOT)/AGESA/Proc/IDS/Family/0x15/"              \
           -I"$(AGESA_ROOT)/AGESA/Proc/IDS/Family/0x15/KV/"           \
           -I"$(AGESA_ROOT)/AGESA/Proc/Common/"                       \
           -I"$(AGESA_ROOT)/AGESA/Proc/CPU/"                          \
           -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/"                   \
           -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/"              \
           -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/KV/"           \
           -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Feature/"                  \
           -I"$(AGESA_ROOT)/AGESA/Proc/Fch/"                          \
           -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Common/"                   \
           -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/"                  \
           -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbCommonLib/"     \
           -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbInitKV/"        \
           -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbNbInitLibV1/"   \
           -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbPcieConfig/"    \
           -I"$(AGESA_ROOT)/AGESA/Proc/Mem/"                          \
           -I"$(AGESA_ROOT)/AGESA/Proc/Mem/Feat/"                     \
           -I"$(AGESA_ROOT)/AGESA/Proc/Mem/Feat/AMP/"                 \
           -I"$(AGESA_ROOT)/AGESA/Proc/Mem/Feat/CRAT/"                \
           -I"$(AGESA_ROOT)/AGESA/Proc/Mem/Feat/IDENDIMM/"            \
           -I"$(AGESA_ROOT)/AGESA/Proc/Mem/NB/"                       \
           -I"$(AGESA_ROOT)/AGESA/Proc/Mem/NB/KV/"                    \
           -I"$(AGESA_ROOT)/AGESA/Proc/Mem/Tech/"                     \
           -I"$(AGESA_ROOT)/AGESA/Proc/Mem/Tech/DDR3/"                \
           -I"$(AGESA_ROOT)/AGESA/Proc/Recovery/CPU/"                 \
           -I"$(AGESA_ROOT)/AGESA/Proc/Topology/"                     \
           -I"$(SOURCE_DIRECTORY)"

# Library paths...
Library_Name ?= AMD
Debug_Library_Path ?= $(AGESA_ROOT)/AGESA/build/gccDebug
Release_Library_Path ?= $(AGESA_ROOT)/AGESA/build/gccRelease

# Build paths...
Debug_Build_Path ?= $(AGESA_ROOT)/AGESA/build/Debug/$(Library_Name)
Release_Build_Path ?= $(AGESA_ROOT)/AGESA/build/Release/$(Library_Name)

# Additional libraries...
Debug_Libraries=
Release_Libraries=

# Preprocessor definitions...
Debug_Preprocessor_Definitions=-D GCC_BUILD -D DEBUG
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


ifeq (y,$(RECURSIVE))

# Subdirectories
subdirectories := $(sort $(dir \
                  $(wildcard ./*/)      \
                  $(wildcard ./*/*.c)   \
                  $(wildcard ./*/*.S)   \
                  ))

subdirectory_names := $(sort $(patsubst %/,%,$(patsubst ./%,%, \
                      $(subdirectories) \
                      )))

sublibraries := $(sort $(patsubst %/,lib$(Library_Name)_%.a,$(patsubst ./%,%, \
                    $(subdirectories)                                         \
                )))

subobjects := $(sort                                                          \
                  $(patsubst %/,                                              \
                          $(Debug_Library_Path)/lib$(Library_Name)_%.a,  \
                      $(patsubst ./%,%,                                       \
                          $(sort $(dir                                        \
                              $(foreach subdirectory,$(subdirectory_names),   \
                                  $(wildcard ./*/*.c) $(wildcard ./*/*.S) )   \
                          ))                                                  \
                      )                                                       \
                  )                                                           \
              )

subincludes := $(sort $(patsubst %/,%,$(patsubst ./%,-I$(SOURCE_DIRECTORY)/%, \
               $(sort $(dir                                                   \
                  $(wildcard ./*/*.hpp)                                       \
                  $(wildcard ./*/*.h)                                         \
                  ))                                                          \
               )))

# macro to define template macros that are used by use_template macro
define create_cc_template
# $1 subdirectory
.NOTPARALLEL: %/lib$(Library_Name)_$(1).a
%/lib$(Library_Name)_$(1).a:
	@if [ -f ./$(1)/$(Library_Name)_$(1).makefile ]; then                     \
		$(MAKE) -s -C ./$(1) -f $(Library_Name)_$(1).makefile               \
				Library_Name=$(Library_Name)_$(1)                             \
				AGESA_ROOT=$(AGESA_ROOT)                                      \
				SOURCE_DIRECTORY=$(SOURCE_DIRECTORY)/$(1)                     \
				INCLUDE="$(INCLUDE) $(subincludes)";                          \
	else                                                                      \
		$(MAKE) -s -C ./$(1) -f $(AGESA_ROOT)/AGESA/binaryPI/generic.makefile     \
				Library_Name=$(Library_Name)_$(1)                             \
				AGESA_ROOT=$(AGESA_ROOT)                                      \
				SOURCE_DIRECTORY=$(SOURCE_DIRECTORY)/$(1)                     \
				INCLUDE="$(INCLUDE) $(subincludes)"                           \
				RECURSIVE=y;                                                  \
	fi
	if [ -r $$@ ]; then                                                       \
		if [ ! -d $$(@D)/$(Library_Name) ]; then                              \
			mkdir $$(@D)/$(Library_Name);                                     \
		fi;                                                                   \
		cd $$(@D)/$(Library_Name);                                            \
		$(AR) x $$@;                                                          \
		rm $$@;                                                               \
	fi

.PHONY: clean-$(1)
clean-$(1):
	@printf "\tCLEAN\t%s\n" $(Library_Name)_$(1)
	@if [ -f ./$(1)/$(Library_Name)_$(1).makefile ]; then                     \
		$(MAKE) clean -s -C ./$(1) -f $(Library_Name)_$(1).makefile           \
				Library_Name=$(Library_Name)_$(1)                             \
				AGESA_ROOT=$(AGESA_ROOT)                                      \
				SOURCE_DIRECTORY=$(SOURCE_DIRECTORY)/$(1);                    \
	else                                                                      \
		$(MAKE) clean -s -C ./$(1) -f $(AGESA_ROOT)/AGESA/binaryPI/generic.makefile \
				Library_Name=$(Library_Name)_$(1)                             \
				AGESA_ROOT=$(AGESA_ROOT)                                      \
				SOURCE_DIRECTORY=$(SOURCE_DIRECTORY)/$(1)                     \
				RECURSIVE=y;                                                  \
	fi

endef

rules := $(foreach subdirectory,$(subdirectory_names),$(eval $(call create_cc_template,$(subdirectory))))
# $(foreach subdirectory,$(subdirectory_names),$(eval $(call create_cc_template,$(subdirectory))))

else

sublibraries ?=

endif

# Builds the Debug configuration...
.PHONY: Debug
-include $(dependencies_debug)
Debug: create_folders $(foreach library,$(sublibraries),$(Debug_Library_Path)/$(library)) $(objects_debug) $(Debug_Implicitly_Linked_Objects)
	@printf "\tLIB\t[debug] $(Library_Name)\n"
	$(AR) rcs $(Debug_Library_Path)/lib$(Library_Name).a                     \
			$(sort                                                           \
				$(objects_debug)                                             \
				$(wildcard $(Debug_Library_Path)/$(Library_Name)/*.o)        \
			)                                                                \
			$(Debug_Implicitly_Linked_Objects)
	if [ -d $(Debug_Library_Path)/$(Library_Name) ]; then                    \
		rm -rf $(Debug_Library_Path)/$(Library_Name);                        \
	fi
	if [ -z "`$(AR) t $(Debug_Library_Path)/lib$(Library_Name).a`" ]; then   \
		rm -f $(Debug_Library_Path)/lib$(Library_Name).a;                    \
	fi

#	set extractions = $(foreach library,                                     \
#		$(sublibraries),                                                     \
#		$(shell                                                              \
#			cd $(Debug_Build_Path);                                          \
#			if [ -f $(Debug_Library_Path)/$(library) ]; then                 \
#				$(AR) vx $(Debug_Library_Path)/$(library) &&                 \
#				rm $(Debug_Library_Path)/$(library);                         \
#			fi                                                               \
#		)                                                                    \
#	)

-include $(dependencies_debug)
$(Debug_Build_Path)/%.o: %.c
	@printf "\tCC\t[debug] $(<F)\n"; \
	$(CC) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -c $< $(INCLUDE) -o $@ -MMD
	$(CC) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -E $< $(INCLUDE) -dM  | sort > $(Debug_Build_Path)/source/$(<F)

$(Debug_Build_Path)/%.o: %.S
	@printf "\tCC\t[debug] $(<F)\n"; \
	$(CC) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -x assembler -c $< $(INCLUDE) -o $@ -MMD
	$(CC) $(Debug_Preprocessor_Definitions) $(Debug_Compiler_Flags) -x assembler -E $< $(INCLUDE) -dM | sort > $(Debug_Build_Path)/source/$(<F)

# Builds the Release configuration...
.PHONY: Release
Release: create_folders $(foreach library,$(sublibraries),$(Release_Library_Path)/$(library)) $(objects_release) $(Release_Implicitly_Linked_Objects)
	@if [ "$?" != "create_folders" ]; then                                   \
		printf "\tLIB\t[release] $(Library_Name)\n";                         \
		$(AR) rcs $(Release_Library_Path)/lib$(Library_Name).a               \
				$(sort                                                       \
					$(objects_release)                                       \
					$(wildcard $(Release_Library_Path)/$(Library_Name)/*.o)  \
				)                                                            \
				$(Release_Implicitly_Linked_Objects);                        \
	fi
	if [ -d $(Release_Library_Path)/$(Library_Name) ]; then                  \
		rm -rf $(Release_Library_Path)/$(Library_Name);                      \
	fi
	if [ -z "`$(AR) t $(Release_Library_Path)/lib$(Library_Name).a`" ]; then \
		rm -f $(Release_Library_Path)/lib$(Library_Name).a;                  \
	fi

-include $(dependencies_release)
$(Release_Build_Path)/%.o: %.c
	@printf "\tCC\t[release] $(<F)\n"; \
	$(CC) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -c $< $(INCLUDE) -o $@ -MMD

$(Release_Build_Path)/%.o: %.S
	@printf "\tCC\t[release] $(<F)\n"; \
	$(CC) $(Release_Preprocessor_Definitions) $(Release_Compiler_Flags) -x assembler -c $< $(INCLUDE) -o $@ -MMD

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p $(Debug_Build_Path)/source
	mkdir -p $(Debug_Library_Path)
	mkdir -p $(Release_Build_Path)/source
	mkdir -p $(Release_Library_Path)

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean: $(foreach subdirectory,$(subdirectory_names),clean-$(subdirectory))
	rm -f $(Debug_Build_Path)/*.o
	rm -f $(Debug_Build_Path)/*.d
	rm -f $(Debug_Library_Path)/$(Library_Name).*
	rm -f $(Debug_Library_Path)/*.so
	rm -f $(Debug_Library_Path)/*.dll
	rm -f $(Debug_Library_Path)/*.exe
	rm -f $(Debug_Library_Path)/*.bin
	rm -f $(Debug_Library_Path)/*.map
	rm -rf $(Debug_Library_Path)/$(Library_Name)
	rm -f $(Release_Build_Path)/*.o
	rm -f $(Release_Build_Path)/*.d
	rm -f $(Release_Library_Path)/$(Library_Name).*
	rm -f $(Release_Library_Path)/*.so
	rm -f $(Release_Library_Path)/*.dll
	rm -f $(Release_Library_Path)/*.exe
	rm -f $(Release_Library_Path)/*.bin
	rm -f $(Release_Library_Path)/*.map
	rm -rf $(Release_Library_Path)/$(Library_Name)

