# Copyright (c) 1994 - 2021 by Vector Informatik GmbH.  All rights reserved.
#
# File    : cn_mingw.tmf
#
# Abstract:
#	Vector CANoe Real-Time Workshop template makefile 
#       
# The following defines can be used to modify the behavior of the build:
#
#    OPTS           - Additional user defines.
#    CPP_OPTS       - C++ compiler options.
#    USER_SRCS      - Additional user sources, such as files needed by
#                     S-functions.
#    USER_INCLUDES  - Additional include paths
#                     (i.e. USER_INCLUDES="/I where-ever1 /I where-ever2")


#------------------------ Macros read by make_rtw -----------------------------
#
# The following macros are read by the Real-Time Workshop build procedure:
#
#  MAKECMD         - This is the command used to invoke the make utility
#  HOST            - What platform this template makefile is targeted for
#                    (i.e. PC or UNIX)
#  BUILD           - Invoke make from the Real-Time Workshop build procedure
#                    (yes/no)?
#  SYS_TARGET_FILE - Name of system target file.

# do not print a warning when seeing legacy tokens
# NO_WARN_LEGACY_TOKENS

MAKECMD              = "C:\Program Files\MATLAB\R2024b\bin/win64/gmake"
HOST                 = PC
BUILD                = yes
SYS_TARGET_FILE      = cn_mingw.tlc
COMPILER_TOOL_CHAIN  = gcc
BUILD_SUCCESS        = ***Created
SHELL                = cmd

#---------------------- Tokens expanded by make_rtw ---------------------------
#
# The following tokens, when wrapped with "|>" and "<|" are expanded by the
# Real-Time Workshop build procedure.
#
#  MODEL_NAME               - Name of the Simulink block diagram
#  MODEL_MODULES            - Any additional generated source modules
#  MAKEFILE_NAME            - Name of makefile created from template makefile <model>.mk
#  MATLAB_ROOT              - Path to were MATLAB is installed.
#  MATLAB_BIN               - Path to MATLAB executable.
#  S_FUNCTIONS              - List of S-functions.
#  S_FUNCTIONS_LIB          - List of S-functions libraries to link.
#  SOLVER                   - Solver source file name
#  NUMST                    - Number of sample times
#  TID01EQ                  - yes (1) or no (0): Are sampling rates of continuous task
#                             (tid=0) and 1st discrete task equal.
#  NCSTATES                 - Number of continuous states
#  BUILDARGS                - Options passed in at the command line.
#  MULTITASKING             - yes (1) or no (0): Is solver mode multitasking
#  EXT_MODE            - yes (1) or no (0): Build for external mode
#  TMW_EXTMODE_TESTING - yes (1) or no (0): Build ext_test.c for external mode
#                        testing.
#  EXTMODE_TRANSPORT   - Index of transport mechanism (e.g. tcpip, serial) for extmode
#  EXTMODE_STATIC      - yes (1) or no (0): Use static instead of dynamic mem alloc.
#  EXTMODE_STATIC_SIZE - Size of static memory allocation buffer.
#  EXPORT_CAPL_FUNCTIONS    - yes (1) or no (0): Are CAPL functions exported
#  PERFORM_SIMSTEP_AT_START - yes (1) or no (0): should simstep be performed at t(0)
#  ENABLE_PARAMETERIZATION  - yes (1) or no (0): Are model parameters accessible from CANoe
#  ENABLE_SIGNALANALYSIS    - yes (1) or no (0): Are Simulink signals accessible from CANoe
#  PERFORMANCE_SYSVARS      - yes (1) or no (0): Are performance sysvars generated
#  CANOEMATLABIF_VER        - CANoe Matlab Interface version information
#  Matlab_VER_GREATER_R2018b - yes (1) or no (0): Matlab Version >R2018b
#  MIP_CONFIG_DISABLE_SYSVAR_WRITE_OPTIMIZATION - yes (on) or no (off): Is the optimization disabled?
#  SUPPRESS_CREATE_SVS      - yes (1) or no (0): Are SVs not defined by CANoe created by the DLL?
#  DEBUG_BUILD              - yes (1) or no (0): Is the DLL built with debug information
#  BUILD_ERT                - yes (1) or no (0): Is the DLL built for Extended Real Time (ERT)
#  TASK_OVERRUN_STRATEGY    - 0: stop simulation; 1: queue tasks; 2: ignore
#  MAX_QUEUED_TASKS         - Max queued task instances before simulation is stopped if overrun strategy is "queue tasks"
#  MIP_CONFIG_IGNORE_NAN_UPDATES - yes (on) or no (off): Ignore consecutive NaN system variable updates
#  MIP_CONFIG_SYSVAR_UPDATE_INTERVAL - Update interval of system variables. Default: -1
#  MIP_CONFIG_IS_APPLICATION_MODEL      - yes (1) or no (0): Is the DLL built for to run without configured canoe networks

MODEL                    = MCU
MODULES                  = rt_matrx.c rt_printf.c rt_logging.c MCU_capi.c MCU_data.c rtGetNaN.c rt_nonfinite.c rt_logging_mmi.c rtw_modelmap_utils.c anorsimstepinitshmem.c sfun_spssw_discc_DSS.c DSS_Utils.c
MAKEFILE                 = MCU.mk
MATLAB_ROOT              = C:\Program Files\MATLAB\R2024b
ALT_MATLAB_ROOT          = C:\PROGRA~1\MATLAB\R2024b
MATLAB_BIN               = C:\Program Files\MATLAB\R2024b\bin
ALT_MATLAB_BIN           = C:\PROGRA~1\MATLAB\R2024b\bin
MATLAB_ARCH_BIN          = $(MATLAB_BIN)/win64
S_FUNCTIONS              = 
S_FUNCTIONS_LIB          = 
SOLVER                   = 
NUMST                    = 2
TID01EQ                  = 0
NCSTATES                 = 0
BUILDARGS                =  MAT_FILE=1 GENERATE_MODEL_DESCRIPTION=1 MIP_CONFIG_XCP_TIMESTAMPS_DISABLE="off" DEBUG_BUILD=0 HAS_WES64_BUILD="off" HAS_WES_BUILD="off" HAS_ERT_BUILD="off" TARGET_ARCHITECTURE="Standard 64 Bit Real-Time DLL" GENERATE_VMODULE=1 COMBINE_OUTPUT_UPDATE_FCNS=0 INCLUDE_MDL_TERMINATE_FCN=1 MULTI_INSTANCE_CODE=0 OPTS="-DTID01EQ=0"
MULTITASKING             = 0
EXT_MODE             = 0
TMW_EXTMODE_TESTING  = 0
EXTMODE_TRANSPORT    = 0
EXTMODE_STATIC       = 0
EXTMODE_STATIC_SIZE  = 1000000
OUTPUT_DIR               = 
EXPORT_CAPL_FUNCTIONS    = 1
PERFORM_SIMSTEP_AT_START = 1
ENABLE_PARAMETERIZATION  = 1
ENABLE_SIGNALANALYSIS    = 1
ENABLE_PARAMETERIZATION_REFMODELS = 1
SHARED_SRC               = 
SHARED_SRC_DIR           = 
SHARED_BIN_DIR           = 
SHARED_LIB               = 
PERFORMANCE_SYSVARS      = 1
SUPPRESS_CREATE_SVS      = 1
VISUAL_VER               = 
CANOEMATLABIF_VER        = 9.3.4
Matlab_VER_GREATER_R2018b = 1
XCP_ENABLE_TIMESTAMP     = 0
MIP_CONFIG_DISABLE_SYSVAR_WRITE_OPTIMIZATION = off
CANOE_SRC                = C:/PROGRA~1/VECTOR~1.2/rtw/c/canoe
CANOE_BIN                = C:/PROGRA~1/VECTOR~1.2/rtw/bin/canoe
SAMPLE_TIMES             = [0.001,0,0]
TASK_OVERRUN_STRATEGY    = 2
MAX_QUEUED_TASKS         = 5
ENABLE_PARAMETERIZATION_EXCLUSION = 0
MIP_CONFIG_IGNORE_NAN_UPDATES = off
MIP_CONFIG_SYSVAR_UPDATE_INTERVAL = -1
MIP_CONFIG_IS_APPLICATION_MODEL      = 0

#--------------------------- Model and reference models -----------------------
MODELREFS                 = 
MODELLIB                  = 
MODELREF_LINK_LIBS        = 
MODELREF_LINK_RSPFILE     = MCU_ref.rsp
MODELREF_INC_PATH         = 
RELATIVE_PATH_TO_ANCHOR   = ..
MODELREF_TARGET_TYPE      = NONE

REQUIRED_VIA_VERSION      = 26

ifneq ("$(MATLAB_ROOT)", "$(ALT_MATLAB_ROOT)")
MATLAB_ROOT = $(ALT_MATLAB_ROOT)
endif
ifneq ("$(MATLAB_BIN)", "$(ALT_MATLAB_BIN)")
MATLAB_BIN = $(ALT_MATLAB_BIN)
endif

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: GNU C Compiler
CC = gcc

# Linker: GNU Linker
LD = gcc

# C++ Compiler: GNU C++ Compiler
CPP = g++

# C++ Linker: GNU C++ Linker
CPP_LD = g++

# Resources compiler: GNU Binary Utilities 
RC = windres

# Archiver: GNU Archiver
AR = ar

#--------------------------------
# Build Configuration
#--------------------------------

ARFLAGS                 = -crus
CFLAGS                  = -c -O0 -msse2
CPPFLAGS                = -c -O0 -std=c++14 -Wno-conversion-null #Wno-conversion-null: mdlparams.cpp
LDFLAGS                 = -Wl,-rpath,"$(MATLAB_ARCH_BIN)",-L"$(MATLAB_ARCH_BIN)" -shared
SHAREDLIB_LDFLAGS       = -shared -Wl,-rpath,"$(MATLAB_ARCH_BIN)",-L"$(MATLAB_ARCH_BIN)" -Wl,--no-undefined #-g
MAKE_FLAGS              = -f $(MAKEFILE)


#---------------------------- CANoe Definitions ------------------------------
FILENAME= $(MODEL)
USER_INCLUDES   = -I$(CANOE_SRC)\devices -I$(CANOE_SRC)\private -I$(CANOE_SRC)\private\Include

PERL = $(MATLAB_ROOT)/sys/perl/win32/bin/perl

ifeq ($(TARGET_ARCH), "x86")
 ifeq ($(MIP_CONFIG_IS_APPLICATION_MODEL), 1)
  DEF = $(CANOE_SRC)/private/VViaApplication.def
 else 
  DEF  = $(CANOE_SRC)/private/nlapml_mingw_x86.def
 endif
endif

# Set the build tool chain
# CANoe_MatlabIntegrationPackage_MinGW_32_bin and MW_MINGW64_LOC environment varables have to be defined!
ifeq ($(TARGET_ARCH), "x86")
  # PATH on the lefthand side is environment varialbe, righthand side is local varialbe
  PATH := $(CANoe_MatlabIntegrationPackage_MinGW_32_bin);$(PATH)
else
  PATH := $(MW_MINGW64_LOC)\bin;$(PATH)
endif

# Export the local variable PATH to environment variable PATH
export PATH


# Get required CANoeEmu library version
ifeq ($(MULTITASKING), 1)
  ifeq ($(BUILD_ERT), 0)
    ifeq ($(DEBUG_BUILD), 1)
      CANOE_LIBS +=  $(CANOE_SRC)/private/lib/CANoeEmu_Mingw530_D.lib
    else
      CANOE_LIBS +=  $(CANOE_SRC)/private/lib/CANoeEmu_Mingw530_R.lib
    endif
  endif
endif

#----------------------------- Defines ----------------------------------------

DEFINES = -D MODEL=$(MODEL) -D "RT" -D NUMST=$(NUMST)   \
          -D TID01EQ=$(TID01EQ) -D NCSTATES=$(NCSTATES) \
          -D MT=$(MULTITASKING) -D "_BUILDNODELAYERDLL" $(EXT_CC_OPTS) $(RTM_CC_OPTS) \
          -D REQUIRED_VIA_VERSION=$(REQUIRED_VIA_VERSION) \

ifeq ($(Multitasking), 1)
  ifeq ($(BUILD_ERT), 0)
    DEFINES += -D TASK_OVERRUN_STRATEGY=$(TASK_OVERRUN_STRATEGY) -D MAX_QUEUED_TASKS=$(MAX_QUEUED_TASKS)
  endif
endif

ifeq ($(RTT_PE), 1)
  DEFINES += -DRTT_PE -D TASK_OVERRUN_STRATEGY=$(TASK_OVERRUN_STRATEGY) -D MAX_QUEUED_TASKS=$(MAX_QUEUED_TASKS)
endif

ifeq ($(strip $(OUTPUT_DIR)),)
  ifeq ($(TARGET_ARCH), "x86")
    OUTPUT_DIR = release
  else
    OUTPUT_DIR = Release64
  endif
endif

ifneq ($(MODELREF_TARGET_TYPE), "NONE")
  ifeq ($(TARGET_ARCH), "x64")
    ifneq ($(RTT_PE), 1)
      MODELLIB_DIR = x64
    else
      MODELLIB_DIR = .
    endif
  else
    MODELLIB_DIR = .
  endif
endif

ifeq ($(EXPORT_CAPL_FUNCTIONS), 1)
  DEFINES += -DEXPORT_CAPL_FUNCTIONS
endif

ifeq ($(PERFORM_SIMSTEP_AT_START), 1)
  DEFINES += -DPERFORM_SIMSTEP_AT_START
endif

ifeq ($(TARGET_ARCH), "x64")
  DEFINES += -DV_OS_64BIT
endif

# XCP_MAX_MSG_LENGTH > 0
ifdef XCP_MAX_MSG_LENGTH
  ifneq ($(XCP_MAX_MSG_LENGTH), -1)
    DEFINES += -DXCP_MAX_MSG_LENGTH=$(XCP_MAX_MSG_LENGTH)
  endif
endif

ifdef PERFORMANCE_SYSVARS
  ifneq ($(PERFORMANCE_SYSVARS), 0)
    DEFINES += -DPERFORMANCE_SYSVARS
  endif
endif

ifdef SUPPRESS_CREATE_SVS
  ifneq ($(SUPPRESS_CREATE_SVS), 0)
    DEFINES += -DSUPPRESS_CREATE_SVS
  endif
endif

ifeq ($(ANALYSIS_MODEL), 1)
  DEFINES += -DANALYSIS_MODEL
endif

ifdef ENABLE_PARAMETERIZATION_EXCLUSION
  ifneq ($(ENABLE_PARAMETERIZATION_EXCLUSION), 0)
    ifeq ($(BUILD_ERT), 0)
      DEFINES += -DENABLE_PARAMETERIZATION_EXCLUSION
    endif
  endif
endif

ifeq ($(XCP_ENABLE_TIMESTAMP), 1)
  DEFINES += -DXCP_ENABLE_TIMESTAMP
endif

ifeq ($(MIP_CONFIG_DISABLE_SYSVAR_WRITE_OPTIMIZATION), on)
  DEFINES += -DMIP_CONFIG_DISABLE_SYSVAR_WRITE_OPTIMIZATION
endif

ifeq ($(RTT_PE), 1)
  DEFINES += -DRTT_PE
endif

ifeq ($(MIP_CONFIG_IGNORE_NAN_UPDATES), on)
  DEFINES += -DIGNORE_NAN_UPDATES
endif

ifneq ($(MIP_CONFIG_SYSVAR_UPDATE_INTERVAL), -1)
ifneq ($(MIP_CONFIG_SYSVAR_UPDATE_INTERVAL), 0)
  DEFINES += -DSYSVAR_UPDATE_INTERVAL=$(MIP_CONFIG_SYSVAR_UPDATE_INTERVAL)
endif
endif

ifeq ($(MIP_CONFIG_IS_APPLICATION_MODEL), 1)
  DEFINES += -DIS_APPLICATION_MODEL
endif

ifeq ($(MODELREF_TARGET_TYPE), NONE)
  ifneq ($(strip $(SHARED_LIB)),)
    ifeq ($(Matlab_VER_GREATER_R2018b), 1)
      ifeq ($(TARGET_ARCH), "x64")
        ifneq ($(RTT_PE), 1)
          SHARED_LIB = ..\slprj\cn_mingw\_sharedutils\x64\rtwshared.lib
        endif
      endif
    endif
  endif
endif
# ---------------------------- Parameter file generation-----------------------
ifeq ($(ENABLE_PARAMETERIZATION), 1)
  ifeq ($(BUILD_ERT), 0)
    ifeq ($(TARGET_ARCH), "x86")
      PARFILES_EXE = $(CANOE_SRC)/ParFiles32.exe
    else
      PARFILES_EXE = $(CANOE_SRC)/ParFiles64.exe
    endif
    CMD_PARAMETER = "$(PARFILES_EXE)" "$(OUTPUT_DIR)\$(FILENAME).dll" "$(CURDIR)" "$(OUTPUT_DIR)"
  endif
  DEFINES += -DENABLE_PARAMETERIZATION
else
  CMD_PARAMETER =
endif

ifeq ($(ENABLE_SIGNALANALYSIS), 1)
  DEFINES += -DENABLE_SIGNALANALYSIS
endif

ifeq ($(ENABLE_PARAMETERIZATION_REFMODELS), 1)
  DEFINES += -DENABLE_PARAMETERIZATION_REFMODELS
endif

#------------------------------ Include/Lib Path ------------------------------
MATLAB_INCLUDES =  -I$(MATLAB_ROOT)\simulink\include
MATLAB_INCLUDES += -I$(MATLAB_ROOT)\extern\include
MATLAB_INCLUDES += -I$(MATLAB_ROOT)\rtw\c\src
MATLAB_INCLUDES += -I$(MATLAB_ROOT)\rtw\c\src\ext_mode\common
MATLAB_INCLUDES += -I$(CANOE_SRC)

# Additional file include paths

MATLAB_INCLUDES +=-I"C:\Users\USER\OneDrive\Desktop\Proj_3\can\CANoe\Simulink"
MATLAB_INCLUDES +=-I"C:\Users\USER\OneDrive\Desktop\Proj_3\can\CANoe\Simulink\MCU_cn_mingw_rtw"
MATLAB_INCLUDES +=-I"$(MATLAB_ROOT)\extern\include"
MATLAB_INCLUDES +=-I"$(MATLAB_ROOT)\simulink\include"
MATLAB_INCLUDES +=-I"$(MATLAB_ROOT)\rtw\c\src"
MATLAB_INCLUDES +=-I"$(MATLAB_ROOT)\rtw\c\src\ext_mode\common"
MATLAB_INCLUDES +=-I"$(MATLAB_ROOT)\toolbox\physmod\powersys\facts\facts"
MATLAB_INCLUDES +=-I"$(MATLAB_ROOT)\toolbox\physmod\powersys\DR\DR"

INCLUDE = -I. -I$(RELATIVE_PATH_TO_ANCHOR) $(MATLAB_INCLUDES)

#------------------------ External mode ---------------------------------------
EXT_SRC     =
EXT_CC_OPTS =
EXT_LIB     =

#------------------------ rtModel ---------------------------------------------
RTM_CC_OPTS = -DUSE_RTMODEL


#----------------------------- Defines ----------------------------------------
MYOPTS   += -D "WIN32" -D "_WINDOWS" -D "_MBCS" -D "_USRDLL" $(OPTS)			
DEFINES  += $(MYOPTS) $(RTM_CC_OPTS)

#----------------- Compiler and Linker Options --------------------------------

ifeq ($(DO_COMPILER_OPTIMIZATION), 1)
  OPT_OPTS = -O2
else
  OPT_OPTS = -O0
endif

ifeq ($(DEBUG_BUILD), 1)
  OPT_OPTS = -O0 -g -D_DEBUG
else
  OPT_OPTS += -DNDEBUG

  # Ennable debug information, for XCP A2L patch process
  ifdef XCP_MAX_MSG_LENGTH
    ifneq ($(XCP_MAX_MSG_LENGTH), -1)
      OPT_OPTS += -g
    endif
  endif
endif

ifeq ($(HAS_CAPL_CALL), 1)
  OPT_OPTS += -fno-omit-frame-pointer
endif

CC_OPTS = $(OPT_OPTS) $(MYOPTS)
CPP_REQ_DEFINES = $(DEFINES)

ASFLAGS              = -c \
                       $(ASFLAGS_ADDITIONAL)

CFLAGS   += $(MODELREF_INC_PATH) $(CC_OPTS) $(CPP_REQ_DEFINES) $(USER_INCLUDES) $(INCLUDE)
CPPFLAGS += $(MODELREF_INC_PATH) $(CPP_OPTS) $(CC_OPTS) $(CPP_REQ_DEFINES) $(USER_INCLUDES) $(INCLUDE)

LDFLAGS  += $(OPT_OPTS) $(DEF)

#----------------------------- Source Files -----------------------------------

# rt_printf.c rt_matrx.c are rquired in any case, but might be already in MODULES
MODULES += rt_printf.c rt_matrx.c 

# Use sort function to remove double entries in MODULES 
MODULES := $(sort $(MODULES)) 

ifeq ($(MODELREF_TARGET_TYPE),NONE)
  PRODUCT_ = $(OUTPUT_DIR)\$(MODEL).dll
  PRODUCT = $(subst :,\:,$(PRODUCT_))
  
  REQ_SRCS  = $(MODEL).c $(MODULES) cn_main.c rt_sim.c \
              $(SOLVER) $(EXT_SRC) \

#XCP_MAX_MSG_LENGTH > 0
  ifdef XCP_MAX_MSG_LENGTH
    ifneq ($(XCP_MAX_MSG_LENGTH), -1)
      REQ_SRCS += xcpBasic.c
    endif
  endif

  ifeq ($(MULTITASKING), 1)
    ifeq ($(BUILD_ERT), 0)
      REQ_SRCS += vtp_tasks.c 
    endif
  endif

  # EXPORT_CAPL_FUNCTIONS == 1 || HAS_CAPL_CALL == 1
  ifeq ($(EXPORT_CAPL_FUNCTIONS), 1)
    REQ_SRCS += caplExport.cpp
  else
    ifeq ($(HAS_CAPL_CALL), 1)
      REQ_SRCS += caplExport.cpp
	endif
  endif
  
  # BUILD_ERT == 0 && ENABLE_PARAMETERIZATION_EXCLUSION == 1
  ifeq ($(BUILD_ERT), 0)
    ifeq ($(ENABLE_PARAMETERIZATION_EXCLUSION), 1)
	    USER_SRCS += ExcludedBlocks.cpp
	  endif
  endif
else #Model Reference Target
  PRODUCT   = $(MODELLIB)
  REQ_SRCS  = $(MODULES) $(EXT_SRC)
endif

CPP_SRC = cncomm.cpp cncommextrablocks.cpp  cn_sfunction_utils.c 
          

ifeq ($(MULTITASKING), 1)
  CPP_SRC += cncommfb.cpp vttapml.cpp CANoeEmu_DllMain.cpp FunctionBusServiceVTT.cpp SignalServiceVTT.cpp SysvarServiceVTT.cpp TimerServiceVTT.cpp ServiceProvider.cpp BasicControlServiceVTT.cpp
else
  ifeq ($(RTT_PE), 0)
    CPP_SRC += BasicControlServiceVIA.cpp VIACommonExports.cpp TimerServiceVIA.cpp cncommfb.cpp cncommdo.cpp nlapml_common.cpp FunctionBusServiceVIA.cpp SignalServiceVIA.cpp SysvarServiceVIA.cpp ServiceProvider.cpp BusServiceVIA.cpp
    ifeq ($(MIP_CONFIG_IS_APPLICATION_MODEL), 1) 
      CPP_SRC += VViaApplication.cpp
    else
      CPP_SRC += nlapml.cpp 
    endif
  else
    CPP_SRC += BasicControlServiceVIA.cpp VIACommonExports.cpp TimerServiceVIA.cpp cncommfb.cpp cncommdo.cpp nlapml_common.cpp FunctionBusServiceVIA.cpp SignalServiceVIA.cpp SysvarServiceVIA.cpp cncommsharedmem.cpp ServiceProvider.cpp BusServiceVIA.cpp SimulationThread.cpp ValueCreatorVIA.cpp cncommfb_common.cpp BusService.cpp
    ifeq ($(MIP_CONFIG_IS_APPLICATION_MODEL), 1) 
      CPP_SRC += VViaApplication.cpp
    else
      CPP_SRC += VIANodelayerApi_RTT_PE.cpp 
    endif
  endif
endif

ifeq ($(ENABLE_PARAMETERIZATION), 1)
  CPP_SRC += mdlparams.cpp
endif
ifeq ($(ENABLE_SIGNALANALYSIS), 1)
  CPP_SRC += mdlsignals.cpp
endif

# XCP_MAX_MSG_LENGTH > 0
ifdef XCP_MAX_MSG_LENGTH
  ifneq ($(XCP_MAX_MSG_LENGTH), -1)
    CPP_SRC += xcpserver.cpp
  endif
endif

# PERFORMANCE_SYSVARS > 0 || $(MULTITASKING) == 1
ifdef PERFORMANCE_SYSVARS
  ifneq ($(PERFORMANCE_SYSVARS), 0)
    CPP_SRC += QPCMeter.cpp
  endif
endif
ifeq ($(MULTITASKING), 1)
  CPP_SRC += QPCMeter.cpp
endif


SRCS = $(REQ_SRCS) $(CPP_SRC) $(USER_SRCS) $(S_FUNCTIONS)

OBJS_CPP_UPPER = $(SRCS:.CPP=.obj)
OBJS_CPP_LOWER = $(OBJS_CPP_UPPER:.cpp=.obj)
OBJS_C_UPPER = $(OBJS_CPP_LOWER:.C=.obj)
OBJS = $(OBJS_C_UPPER:.c=.obj) 

ifeq ($(TARGET_ARCH), "x64")
  OBJS += GetParamMingwX64.obj
endif

SHARED_SRC := $(wildcard $(SHARED_SRC_DIR)/*.c)
SHARED_OBJS_NO_PATH = $(addsuffix .o, $(basename $(notdir $(SHARED_SRC))))
SHARED_OBJS = $(addprefix $(join $(SHARED_BIN_DIR), /), $(SHARED_OBJS_NO_PATH))

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS = -static -lstdc++ -lws2_32


# ------------------------- Additional Libraries ------------------------------

LIBS =


LIBS += $(CANOE_LIBS)

#--------------------------- Resources ----------------------------------------

#--------------------------------- Rules --------------------------------------
# Standalone executable
ifeq ($(MODELREF_TARGET_TYPE), NONE)
$(PRODUCT) : clean $(OBJS) $(MODELREF_LINK_LIBS) $(SHARED_LIB) $(LIBS)
ifeq ($(TARGET_ARCH), "x64")
    ifneq ($(RTT_PE), 1)
			$(PERL) $(CANOE_SRC)\copyx64libs.pl $(MODELREF_LINK_RSPFILE) $(RELATIVE_PATH_TO_ANCHOR) $(SYS_TARGET_FILE)
    else
			$(PERL) $(CANOE_SRC)\copyrefmodellibs.pl $(MODELREF_LINK_LIBS)
    endif
else
	$(PERL) $(CANOE_SRC)\copyrefmodellibs.pl $(MODELREF_LINK_LIBS)
endif
	@echo ### Creating $@ ...
	@if not exist "$(OUTPUT_DIR)/$(NULL)" mkdir "$(OUTPUT_DIR)"
	@$(LD) $(LDFLAGS) -o$@  $(OBJS) $(SHARED_LIB) $(notdir $(MODELREF_LINK_LIBS)) $(LIBS) $(MAT_LIBS) $(SYSTEM_LIBS) $(S_FUNCTIONS_LIB)
	@echo ***Created $@

	$(CMD_PARAMETER)
else
# Model Reference Target
$(PRODUCT) : clean $(OBJS) $(MODELREF_LINK_LIBS)
	@echo ### Creating $@ ...
	@if not exist "$(MODELLIB_DIR)/$(NULL)" mkdir "$(MODELLIB_DIR)"
ifeq ($(TARGET_ARCH), "x64")
    ifneq ($(RTT_PE), 1)
			$(PERL) $(CANOE_SRC)\copyx64libs.pl $(MODELREF_LINK_RSPFILE) $(RELATIVE_PATH_TO_ANCHOR) $(SYS_TARGET_FILE)
    else
			$(PERL) $(CANOE_SRC)\copyrefmodellibs.pl $(MODELREF_LINK_LIBS)
    endif
else
	$(PERL) $(CANOE_SRC)\copyrefmodellibs.pl $(MODELREF_LINK_LIBS)
endif
	@$(AR) rcs $(MODELLIB_DIR)\$(MODELLIB) $(OBJS) 
	@echo ***Created $@
endif

ifneq ($(SHARED_SRC_DIR),)
$(SHARED_BIN_DIR)/%.o : $(SHARED_SRC_DIR)/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -c -o $@ $<
endif

%.obj : $(CANOE_SRC)/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(CANOE_SRC)/private/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(CANOE_SRC)/private/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"

%.obj : $(CANOE_SRC)/devices/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(CANOE_SRC)/devices/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"

%.obj : $(CANOE_SRC)/devices/%.s
	@echo Compile: $<
	$(AS) $(ASFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/rtw/c/src/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/rtw/c/src/ext_mode/common/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/rtw/c/src/ext_mode/tcpip/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/rtw/c/src/ext_mode/serial/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/rtw/c/src/ext_mode/custom/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"


%.obj : C:\PROGRA~1\VECTOR~1.2\rtw\c\canoe\devices/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : C:\PROGRA~1\VECTOR~1.2\rtw\c\canoe\devices/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)\toolbox\physmod\powersys\powersys/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)\toolbox\physmod\powersys\powersys/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)\rtw\c\src/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)\rtw\c\src/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)\simulink\src/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)\simulink\src/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)\toolbox\simulink\blocks\src/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)\toolbox\simulink\blocks\src/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"


%.obj : $(MATLAB_ROOT)/simulink/src/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/simulink/src/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/toolbox/comm/commsfun/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/toolbox/comm/commsfun/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/toolbox/dspblks/dspmex/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/toolbox/dspblks/dspmex/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/toolbox/fixpoint/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/toolbox/fixpoint/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/toolbox/fuzzy/fuzzy/src/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(MATLAB_ROOT)/toolbox/fuzzy/fuzzy/src/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"

%.obj : $(RELATIVE_PATH_TO_ANCHOR)/%.c
	@echo Compile: $<
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : $(RELATIVE_PATH_TO_ANCHOR)/%.cpp
	@echo Compile: $<
	$(CPP) $(CPPFLAGS) -o"$@" "$<"

%.obj : %.c
	@echo Compile: $< 
	$(CC) $(CFLAGS) -o"$@" "$<"

%.obj : %.cpp
	@echo Compile: $< 
	$(CC) $(CPPFLAGS) -o"$@" "$<"

$(CANOE_SRC)%.res : .rc
	@echo Compile: $<
	$(RC) $(RCFLAGS) -o"$@" "$<"

vtp_tasks.c:
	"$(CANOE_BIN)/genvtp/genvtp.exe" -name $(MODEL) -numst $(NUMST) -st $(SAMPLE_TIMES)

.phony: clean

clean:
	del *.obj >nul 2>&1
ifeq ($(MULTITASKING), 1)
ifeq ($(BUILD_ERT), 0)
	del vtp_tasks.c >nul 2>&1
endif
endif
# remove this line for verbose mode
.SILENT:
 
#----------------------------- Libraries -------------------------------------- 
 



#----------------------------- Dependencies -----------------------------------

$(SHARED_LIB) : $(SHARED_OBJS)
	@echo ### Creating $@
	$(AR) ruvs $@ $(SHARED_OBJS)
	@echo ### $@ Created