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

.NOTPARALLEL: Agesa_B1_Target Agesa_B2_Target

AGESA_ROOT := $(CURDIR)/..

Includes += -I"."
Includes += -I"$(AGESA_ROOT)/AGESA/build/"
Includes += -I"$(AGESA_ROOT)/AGESA/binaryPI/"
Includes += -I"$(AGESA_ROOT)/AGESA/"
Includes += -I"$(AGESA_ROOT)/AGESA/Lib/"
Includes += -I"$(AGESA_ROOT)/AGESA/Include/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/IDS/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/IDS/Family/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/IDS/Family/0x15/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/IDS/Family/0x15/KV/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/Common/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/CPU/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Feature/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/Fch/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Common/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/HT/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/Mem/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/Recovery/CPU/"
Includes += -I"$(AGESA_ROOT)/AGESA/Proc/Topology/"
Includes += -I"$(PWD)"

CFLAGS :=
CFLAGS += -Wall -Wundef -Wstrict-prototypes -Wmissing-prototypes
CFLAGS += -Wwrite-strings -Wredundant-decls -Wno-trigraphs
CFLAGS += -Wstrict-aliasing -Wshadow
CFLAGS += -pipe
CFLAGS += -march=barcelona -m32 -masm=att
CFLAGS += -fno-common -ffreestanding -fno-builtin -fomit-frame-pointer
CFLAGS += -fno-zero-initialized-in-bss
export CFLAGS


export AGESA_INC  := $(Includes)

# Builds all the projects in the solution...
INCLUDE += $(Includes)

export INCLUDE := $(INCLUDE)

.PHONY: all_projects
all_projects:  AMD_binaryPI AMD_Proc AMD_Proc_Common AMD_Proc_CPU AMD_Proc_Fch AMD_Proc_GNB AMD_Proc_IDS AMD_Proc_Mem AMD_Proc_Topology AMD AMD_Lib

# Builds project 'Agesa_B1_Target'...
#.PHONY: Agesa_B1_Target
#Agesa_B1_Target: $(all_projects)
#	$(MAKE) -s --directory="./Legacy/Proc/" --file=$(AGESA_ROOT)/AGESA/binaryPI/Agesa_B1_Target.makefile

# Builds project 'Agesa_B2_Target'...
.PHONY: Agesa_B2_Target
Agesa_B2_Target: $(all_projects)
	@$(MAKE) -s --directory="./Legacy/Proc/" --file=$(AGESA_ROOT)/AGESA/binaryPI/Agesa_B2_Target.makefile

# Builds project 'AMD_binaryPI'...
.PHONY: AMD_binaryPI
AMD_binaryPI:
	@$(MAKE) -s --directory="./binaryPI/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=$(@F) AGESA_ROOT=$(AGESA_ROOT) SOURCE_DIRECTORY="." RECURSIVE=n \
	        INCLUDE='$(INCLUDE) \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/IDS/Debug/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbInitKV/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbInitTN/"'

# Builds project 'AMD'...
.PHONY: AMD
AMD:
	@$(MAKE) -s --directory="." --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=$(@F) AGESA_ROOT=$(AGESA_ROOT) SOURCE_DIRECTORY="." RECURSIVE=n

# Builds project 'AMD_Lib'...
.PHONY: AMD_Lib
AMD_Lib:
	@$(MAKE) -s --directory="./Lib/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=$(@F) AGESA_ROOT=$(AGESA_ROOT) SOURCE_DIRECTORY="./Lib" RECURSIVE=n

# Builds project 'AMD_Proc'...
.PHONY: AMD_Proc
AMD_Proc:
	@$(MAKE) -s --directory="./Proc/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=$(@F) AGESA_ROOT=$(AGESA_ROOT) SOURCE_DIRECTORY="./Proc" RECURSIVE=n

# Builds project 'AMD_Proc_Common'...
.PHONY: AMD_Proc_Common
AMD_Proc_Common:
	@$(MAKE) -s --directory="./Proc/Common/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=$(@F) AGESA_ROOT=$(AGESA_ROOT) SOURCE_DIRECTORY="./Proc/Common" RECURSIVE=y

# Builds project 'AMD_Proc_CPU'...
.PHONY: AMD_Proc_CPU
AMD_Proc_CPU:
	@$(MAKE) -s --directory="./Proc/CPU/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=$(@F) AGESA_ROOT=$(AGESA_ROOT) SOURCE_DIRECTORY="./Proc/CPU" RECURSIVE=y \
	        INCLUDE='$(INCLUDE) \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/IDS/Family/0x15/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/IDS/Family/0x15/KV/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/IDS/Family/0x15/TN/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/KV/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/TN/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Common/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbCommonLib/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbPcieConfig/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbInitKV/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbInitTN/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/Mem/Feat/CRAT/"'

# Builds project 'AMD_Proc_Fch'...
.PHONY: AMD_Proc_Fch
AMD_Proc_Fch:
	@$(MAKE) -s --directory="./Proc/Fch/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=$(@F) RECURSIVE=y \
	        INCLUDE='$(INCLUDE) -I"$(AGESA_ROOT)/AGESA/Proc/Fch/Common/"'

# Builds project 'AMD_Proc_GNB'...
.PHONY: AMD_Proc_GNB
AMD_Proc_GNB:
	@$(MAKE) -s --directory="./Proc/GNB/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=$(@F) RECURSIVE=y \
	        INCLUDE='$(INCLUDE) \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/KV/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/TN/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Common/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbCommonLib/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbPcieConfig/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbGfxConfig/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbGfxInitLibV1/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbPcieInitLibV1/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbPcieInitLibV4/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbPcieInitLibV5/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbNbInitLibV1/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbNbInitLibV4/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbNbInitLibV5/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbSmuLibV7/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbPcieAspm/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbSbLib/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbIvrsLib/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbSbIommuLib/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbIommuIvrs/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbTable/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbGfxIntTableV3/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbPcieTrainingV1/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbPcieTrainingV2/"'

# Builds project 'AMD_Proc_IDS'...
.PHONY: AMD_Proc_IDS
AMD_Proc_IDS:
	@$(MAKE) -s --directory="./Proc/IDS/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=$(@F) RECURSIVE=y \
	        INCLUDE='$(INCLUDE) \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/KV/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/TN/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Common/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbCommonLib/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbInitKV/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbInitTN/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbPcieConfig/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/IDS/Debug/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/IDS/Library/"'

# Builds project 'AMD_Proc_Recovery'...
.PHONY: AMD_Proc_Recovery
AMD_Proc_Recovery:
	@$(MAKE) -s --directory="./Proc/Recovery/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=$(@F) RECURSIVE=y \
	        INCLUDE='$(INCLUDE) \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/Recovery/Mem/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbCommonLib/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbNbInitLibV1/"'

# Builds project 'AMD_Proc_Mem'...
.PHONY: AMD_Proc_Mem
AMD_Proc_Mem:
	@$(MAKE) -s --directory="./Proc/Mem/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=$(@F) RECURSIVE=y \
	        INCLUDE='$(INCLUDE) \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/Mem/Tech/DDR3/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/Mem/Feat/IDENDIMM/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/Mem/Main/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/Mem/NB/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/Mem/NB/KV/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/Mem/NB/TN/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/KV/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/CPU/Family/0x15/TN/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Common/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbCommonLib/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbInitKV/" \
	                 -I"$(AGESA_ROOT)/AGESA/Proc/GNB/Modules/GnbInitTN/"'

# Builds project 'AMD_Proc_Topology'...
.PHONY: AMD_Proc_Topology
AMD_Proc_Topology:
	@$(MAKE) -s --directory="./Proc/Topology/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=$(@F) RECURSIVE=y

.PHONY: all
all: all_projects


.PHONY: clean-root
clean-root:
	@$(MAKE) -s clean --directory="."                --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=AMD               AGESA_ROOT=$(AGESA_ROOT) RECURSIVE=n

PHONY: clean-lib
clean-lib:
	@$(MAKE) -s clean --directory="./Lib/"           --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=AMD_Lib           AGESA_ROOT=$(AGESA_ROOT) RECURSIVE=n

PHONY: clean-proc
clean-proc:
	@$(MAKE) -s clean --directory="./Proc/"          --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=AMD_Proc          AGESA_ROOT=$(AGESA_ROOT) RECURSIVE=n

PHONY: clean-common
clean-common:
	@$(MAKE) -s clean --directory="./Proc/Common/"   --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=AMD_Proc_Common   AGESA_ROOT=$(AGESA_ROOT) RECURSIVE=y

PHONY: clean-cpu
clean-cpu:
	@$(MAKE) -s clean --directory="./Proc/CPU/"      --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=AMD_Proc_CPU      AGESA_ROOT=$(AGESA_ROOT) RECURSIVE=y

PHONY: clean-fch
clean-fch:
	@$(MAKE) -s clean --directory="./Proc/Fch/"      --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=AMD_Proc_Fch      AGESA_ROOT=$(AGESA_ROOT) RECURSIVE=y

PHONY: clean-gnb
clean-gnb:
	@$(MAKE) -s clean --directory="./Proc/GNB/"      --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=AMD_Proc_GNB      AGESA_ROOT=$(AGESA_ROOT) RECURSIVE=y

PHONY: clean-ids
clean-ids:
	@$(MAKE) -s clean --directory="./Proc/IDS/"      --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=AMD_Proc_IDS      AGESA_ROOT=$(AGESA_ROOT) RECURSIVE=y

PHONY: clean-recovery
clean-recovery:
	@$(MAKE) -s clean --directory="./Proc/Recovery/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=AMD_Proc_Recovery AGESA_ROOT=$(AGESA_ROOT) RECURSIVE=y

PHONY: clean-mem
clean-mem:
	@$(MAKE) -s clean --directory="./Proc/Mem/"      --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=AMD_Proc_Mem      AGESA_ROOT=$(AGESA_ROOT) RECURSIVE=y

PHONY: clean-topology
clean-topology:
	@$(MAKE) -s clean --directory="./Proc/Topology/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=AMD_Proc_Topology AGESA_ROOT=$(AGESA_ROOT) RECURSIVE=y

PHONY: clean-pi
clean-pi:
	@$(MAKE) -s clean --directory="./binaryPI/" --file=$(AGESA_ROOT)/AGESA/binaryPI/generic.makefile Library_Name=AMD_binaryPI           AGESA_ROOT=$(AGESA_ROOT) RECURSIVE=n

# Cleans all projects...
.PHONY: clean
clean: clean-pi clean-root clean-lib clean-proc clean-common clean-cpu clean-fch clean-gnb clean-ids clean-recovery clean-mem clean-topology

