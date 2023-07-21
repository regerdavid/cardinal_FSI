# ======================================================================================
# Cardinal Makefile
# ======================================================================================
#
# Optional environment variables:
#
# To control where various third-party dependencies are. You don't need to set
# any of these unless you want to use non-submodule third-party dependencies:

# * CONTRIB_DIR      : Dir with third-party dependencies (default: contrib)
# * MOOSE_SUBMODULE  : Top-level MOOSE dir (default: $(CONTRIB_DIR)/moose)
# * NEKRS_DIR        : Top-level NekRS dir (default: $(CONTRIB_DIR)/nekRS)
# * OPENMC_DIR       : Top-level OpenMC dir (default: $(CONTRIB_DIR)/openmc)
# * DAGMC_DIR        : Top-level DagMC dir (default: $(CONTRIB_DIR)/DAGMC)
# * MOAB_DIR         : Top-level Moab dir (default: $(CONTRIB_DIR)/moab)
# * BISON_DIR        : Top-level Bison dir (default: $(CONTRIB_DIR)/bison)
# * SAM_DIR          : Top-level SAM dir (default: $(CONTRIB_DIR)/SAM)
# * SOCKEYE_DIR      : Top-level Sockeye dir (default: $(CONTRIB_DIR)/sockeye)
# * SODIUM_DIR       : Top-level sodium dir (default: $(CONTRIB_DIR)/sodium)
# * POTASSIUM_DIR    : Top-level potassium dir (default: $(CONTRIB_DIR)/potassium)
# * IAPWS95_DIR      : Top-level iapws95 dir (default: $(CONTRIB_DIR)/iapws95)

# * EIGEN3_DIR       : Top-level eigen3 dir (should contain FindEigen3.cmake). This
#                      is only needed if enabling DagMC.

# To control where OpenMC grabs HDF5 from; you don't need to set any of these unless
# you don't want to use the HDF5 that comes with PETSc

# * HDF5_ROOT         : Top-level HDF5 directory (default: $(PETSC_DIR)/$(PETSC_ARCH), meaning that
#                       the default is to use HDF5 downloaded by PETSc). This makefile
#                       will then get the header files from $(HDF5_ROOT)/include and the
#                       libraries from $(HDF5_ROOT)/lib.
# * HDF5_INCLUDE_DIR  : Top-level HDF5 header dir (default: $(HDF5_ROOT)/include)
# * HDF5_LIBDIR       : Top-level HDF5 lib dir (default: $(HDF5_ROOT)/lib)
# * PETSC_DIR         : Top-level PETSc dir (default: $(MOOSE_SUBMODULE)/petsc)
# * PETSC_ARCH        : PETSc architecture (default: arch-moose)
# ======================================================================================

# Whether you want to build with NekRS; set to anything except 'yes' to skip
ENABLE_NEK          ?= yes

# Whether you want to build with OpenMC; set to anything except 'yes' to skip
ENABLE_OPENMC       ?= no

# Whether you want to build OpenMC with DAGMC support; set to anything except 'yes' to skip
ENABLE_DAGMC        ?= no

#Whether you want ot build with Diablo
ENABLE_DIABLO       = yes

##CARDINAL_DIR      
CARDINAL_DIR        := $(abspath $(dir $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))))
CONTRIB_DIR         := $(CARDINAL_DIR)/contrib
MOOSE_SUBMODULE     ?= $(CONTRIB_DIR)/moose
NEKRS_DIR           ?= $(CONTRIB_DIR)/nekRS
#OPENMC_DIR          ?= $(CONTRIB_DIR)/openmc
DAGMC_DIR           ?= $(CONTRIB_DIR)/DAGMC
MOAB_DIR            ?= $(CONTRIB_DIR)/moab
PETSC_DIR           ?= $(MOOSE_SUBMODULE)/petsc
PETSC_ARCH          ?= arch-moose
LIBMESH_DIR         ?= $(MOOSE_SUBMODULE)/libmesh/installed/
CONTRIB_INSTALL_DIR ?= $(CARDINAL_DIR)/install
BISON_DIR           ?= $(CONTRIB_DIR)/bison
SAM_DIR             ?= $(CONTRIB_DIR)/SAM
SOCKEYE_DIR         ?= $(CONTRIB_DIR)/sockeye
SODIUM_DIR          ?= $(CONTRIB_DIR)/sodium
POTASSIUM_DIR       ?= $(CONTRIB_DIR)/potassium
IAPWS95_DIR         ?= $(CONTRIB_DIR)/iapws95
DIABLO_DIR	    = $(CONTRIB_DIR)/diablo

# This is the Eigen3 location on CIVET. If you are using MOOSE's conda environment,
# you don't need to set these variables, because conda sets them for you. The only
# scenario where you might need to manually set these is if you're not using the
# conda environment. You will get a compile error about FindEigen3.cmake if you
# do indeed need to set these.
EIGEN3_DIR          ?= $(LIBMESH_DIR)/include
Eigen3_DIR          ?= $(EIGEN3_DIR)

# If HDF5_ROOT is set, use those settings to link HDF5 to OpenMC.
# Otherwise, use where PETSc will put HDF5 if downloading it.
ifeq ($(HDF5_ROOT),)
  HDF5_ROOT          := $(PETSC_DIR)/$(PETSC_ARCH)
  export HDF5_ROOT
endif

HDF5_INCLUDE_DIR    ?= $(HDF5_ROOT)/include
HDF5_LIBDIR         ?= $(HDF5_ROOT)/lib

DIABLO_ROOT = $(DIABLO_DIR)/Build/toss_4_x86_64_ib_gcc-12.1.1_mvapich2-2.3.7_debug
DIABLO_INCLUDE_DIR = $(DIABLO_ROOT)/include
DIABLO_SOURCE_INCLUDE_DIR = $(DIABLO_DIR)/Source
DIABLO_OTHER_INCLUDE_DIR = $(DIABLO_DIR)/toss_4_x86_64_ib_small/gcc12.1.1/opt/include
DIABLO_LIBDIR = $(DIABLO_ROOT)
DIABLO_EXTERNAL_LIBDIR = $(DIABLO_DIR)/toss_4_x86_64_ib_small/gcc12.1.1/opt/lib
DIABLO_LIB = Diablo_debug
#DIABLOSUB_LIB	    ?= diablo_sub_debug
DIABLO_PETSCDIR = $(PETSC_DIR)/$(PETSC_ARCH)
LIB01 = HYPRE
LIB02 = lis
LIB03 = parpack
LIB04 = dmumps
LIB05 = mumps_common
LIB06 = lapack
LIB07 = scalapack
LIB08 = parmetis
LIB09 = metis
LIB10 = exoIIv2for
LIB11 = exodus
LIB12 = siloh5
LIB13 = netcdf
LIB14 = hdf5_fortran
LIB15 = hdf5_hl
LIB16 = hdf5
LIB17 = pord
LIB18 = lua
LIB19 = mili
LIB20 = nlopt
LIB21 = curl
LIB22 = arpack
LIB23 = pwsmp_gcc8.3.1_mvapich22.3_2021-09

DIABLO_EXTRA_LIBS =-l$(LIB01) -l$(LIB02) -l$(LIB03) -l$(LIB04) -l$(LIB05) -l$(LIB06) -l$(LIB07) -l$(LIB08) -l$(LIB09) -l$(LIB10) -l$(LIB11) -l$(LIB12) -l$(LIB13) -l$(LIB14) -l$(LIB15) -l$(LIB16) -l$(LIB17) -l$(LIB18) -l$(LIB19) -l$(LIB20) -l$(LIB21) -l$(LIB22) -l$(LIB23)
DIABLO_EXTRA_LIBS_WL = -Wl,-l$(LIB01) -Wl,-l$(LIB02) -Wl,-l$(LIB03) -Wl,-l$(LIB04) -Wl,-l$(LIB05) -Wl,-l$(LIB06) -Wl,-l$(LIB07) -Wl,-l$(LIB08) -Wl,-l$(LIB09) -Wl,-l$(LIB10) -Wl,-l$(LIB11) -Wl,-l$(LIB12) -Wl,-l$(LIB13) -Wl,-l$(LIB14) -Wl,-l$(LIB15) -Wl,-l$(LIB16) -Wl,-l$(LIB17) -Wl,-l$(LIB18), -Wl,-l$(LIB19) -Wl,-l$(LIB20) -Wl,-l$(LIB21) -Wl,-l$(LIB22) -Wl,-l$(LIB23)

# convert ENABLE_NEK, ENABLE_OPENMC, and ENABLE_DAGMC to consistent truthy value
ifeq ($(ENABLE_OPENMC),$(filter $(ENABLE_OPENMC), true yes on 1 TRUE YES ON))
  ENABLE_OPENMC := yes
endif
ifeq ($(ENABLE_NEK),$(filter $(ENABLE_NEK), true yes on 1 TRUE YES ON))
  ENABLE_NEK := yes
endif
ifeq ($(ENABLE_DAGMC),$(filter $(ENABLE_DAGMC), true yes on 1 TRUE YES ON))
  ENABLE_DAGMC := yes
endif

ifeq ($(ENABLE_OPENMC), yes)
  # HDF5 is only needed to be linked if using OpenMC
  $(info Cardinal is using HDF5 from $(HDF5_ROOT))
else
  ifeq ($(ENABLE_DAGMC), yes)
    $(info Ignoring ENABLE_DAGMC because OpenMC is not enabled.)
    ENABLE_DAGMC := no
  endif
endif

# Check that NEKRS_HOME is set to the correct location
ifeq ($(ENABLE_NEK), yes)
  include config/check_nekrs.mk
endif

ALL_MODULES         := no

FLUID_PROPERTIES    := yes
HEAT_CONDUCTION     := yes
NAVIER_STOKES       := yes
REACTOR             := yes
STOCHASTIC_TOOLS    := yes
TENSOR_MECHANICS    := yes
THERMAL_HYDRAULICS  := yes

# Perform various checks on the dependencies: (i) error if dependencies are
# missing or conflict with one another; (ii) warn if not using a paired
# submodule hash; (iii) check that NEKRS_HOME points to the correct location
include config/check_deps.mk

# BUILD_TYPE will be passed to CMake via CMAKE_BUILD_TYPE
ifeq ($(METHOD),dbg)
	BUILD_TYPE := Debug
else
	BUILD_TYPE := Release
endif

OCCA_CUDA_ENABLED=0
OCCA_HIP_ENABLED=0
OCCA_OPENCL_ENABLED=0

DAGMC_BUILDDIR := $(CARDINAL_DIR)/build/DAGMC
DAGMC_INSTALL_DIR := $(CONTRIB_INSTALL_DIR)

MOAB_BUILDDIR := $(CARDINAL_DIR)/build/moab
MOAB_INSTALL_DIR := $(CONTRIB_INSTALL_DIR)

NEKRS_BUILDDIR := $(CARDINAL_DIR)/build/nekrs
NEKRS_INSTALL_DIR := $(CONTRIB_INSTALL_DIR)
NEKRS_INCLUDES := \
	-I$(NEKRS_DIR)/src \
	-I$(NEKRS_DIR)/src/bdry \
	-I$(NEKRS_DIR)/src/bench/advsub \
	-I$(NEKRS_DIR)/src/bench/axHelm \
	-I$(NEKRS_DIR)/src/bench/core \
	-I$(NEKRS_DIR)/src/bench/fdm \
	-I$(NEKRS_DIR)/src/cds \
	-I$(NEKRS_DIR)/src/core \
	-I$(NEKRS_DIR)/src/findpts \
	-I$(NEKRS_DIR)/src/io \
	-I$(NEKRS_DIR)/src/lib \
	-I$(NEKRS_DIR)/src/linAlg \
	-I$(NEKRS_DIR)/src/mesh \
	-I$(NEKRS_DIR)/src/navierStokes \
	-I$(NEKRS_DIR)/src/nekInterface \
	-I$(NEKRS_DIR)/src/neknek \
	-I$(NEKRS_DIR)/src/plugins \
	-I$(NEKRS_DIR)/src/pointInterpolation \
	-I$(NEKRS_DIR)/src/postProcessing \
	-I$(NEKRS_DIR)/src/regularization \
	-I$(NEKRS_DIR)/src/setup \
	-I$(NEKRS_DIR)/src/solvers/cvode \
	-I$(NEKRS_DIR)/src/solvers/elliptic \
	-I$(NEKRS_DIR)/src/solvers/elliptic/linearSolver \
	-I$(NEKRS_DIR)/src/solvers/elliptic/MG \
	-I$(NEKRS_DIR)/src/udf \
	-I$(NEKRS_DIR)/src/utils \
	-I$(NEKRS_INSTALL_DIR)/gatherScatter \
	-I$(NEKRS_INSTALL_DIR)/include \
	-I$(NEKRS_INSTALL_DIR)/libparanumal/include \
	-I$(NEKRS_INSTALL_DIR)/include/libP/parAlmond \
	-I$(NEKRS_INSTALL_DIR)/include/linAlg
NEKRS_LIBDIR := $(NEKRS_INSTALL_DIR)/lib
NEKRS_LIB := $(NEKRS_LIBDIR)/libnekrs.so
# This needs to be exported
 export NEKRS_HOME=$(CARDINAL_DIR)

OPENMC_BUILDDIR := $(CARDINAL_DIR)/build/openmc
OPENMC_INSTALL_DIR := $(CONTRIB_INSTALL_DIR)
OPENMC_INCLUDES := -I$(OPENMC_INSTALL_DIR)/include
OPENMC_LIBDIR := $(OPENMC_INSTALL_DIR)/lib
OPENMC_LIB := $(OPENMC_LIBDIR)/libopenmc.so

# This is used in $(FRAMEWORK_DIR)/build.mk
HDF5_INCLUDES       := -I$(HDF5_INCLUDE_DIR) -I$(HDF5_ROOT)/include
#DIABLO_INCLUDES	    := -I$(DIABLO_INCLUDE_DIR) -I$(DIABLO_SOURCE_INCLUDE_DIR) -I$(DIABLO_OTHER_INCLUDE_DIR)
DIABLO_INCLUDES	    := -I$(DIABLO_INCLUDE_DIR) -I$(DIABLO_OTHER_INCLUDE_DIR)
# ======================================================================================
# PETSc
# ======================================================================================

# Use compiler info discovered by PETSC
ifeq ($(PETSC_ARCH),)
	include $(PETSC_DIR)/$(PETSC_ARCH)/lib/petsc/conf/petscvariables
else
	include $(PETSC_DIR)/lib/petsc/conf/petscvariables
endif

# ======================================================================================
# MOOSE core objects
# ======================================================================================

# Use the MOOSE submodule if it exists and MOOSE_DIR is not set
ifneq ($(wildcard $(MOOSE_SUBMODULE)/framework/Makefile),)
	MOOSE_DIR        ?= $(MOOSE_SUBMODULE)
else
	MOOSE_DIR        ?= $(shell dirname `pwd`)/moose
endif

# framework
FRAMEWORK_DIR      := $(MOOSE_DIR)/framework
include $(FRAMEWORK_DIR)/build.mk
include $(FRAMEWORK_DIR)/moose.mk

# ======================================================================================
# MOOSE modules
# ======================================================================================

include $(MOOSE_DIR)/modules/modules.mk

# Bison submodule
ifneq ($(BISON_CONTENT),)
  libmesh_CXXFLAGS    += -DENABLE_BISON_COUPLING
  APPLICATION_DIR     := $(BISON_DIR)
  APPLICATION_NAME    := bison
  include             $(FRAMEWORK_DIR)/app.mk
endif

# SAM submodule
ifneq ($(SAM_CONTENT),)
  libmesh_CXXFLAGS    += -DENABLE_SAM_COUPLING
  APPLICATION_DIR     := $(SAM_DIR)
  APPLICATION_NAME    := sam
  TENSOR_MECHANICS    := yes
  include             $(FRAMEWORK_DIR)/app.mk
endif

# Sockeye submodule
ifneq ($(SOCKEYE_CONTENT),)
  libmesh_CXXFLAGS    += -DENABLE_SOCKEYE_COUPLING
  APPLICATION_DIR     := $(SOCKEYE_DIR)
  APPLICATION_NAME    := sockeye
  include             $(FRAMEWORK_DIR)/app.mk
endif

# sodium submodule
ifneq ($(SODIUM_CONTENT),)
  libmesh_CXXFLAGS    += -DENABLE_SODIUM
  APPLICATION_DIR     := $(SODIUM_DIR)
  APPLICATION_NAME    := sodium
  include             $(FRAMEWORK_DIR)/app.mk
  include             $(SODIUM_DIR)/libSodiumProperties.mk
endif

# potassium submodule
ifneq ($(POTASSIUM_CONTENT),)
  libmesh_CXXFLAGS    += -DENABLE_POTASSIUM
  APPLICATION_DIR     := $(POTASSIUM_DIR)
  APPLICATION_NAME    := potassium
  include             $(FRAMEWORK_DIR)/app.mk
  include             $(POTASSIUM_DIR)/libPotassiumProperties.mk
endif

# iapws95 submodule
ifneq ($(IAPWS95_CONTENT),)
  libmesh_CXXFLAGS    += -DENABLE_IAPWS95
  APPLICATION_DIR     := $(IAPWS95_DIR)
  APPLICATION_NAME    := iapws95
  include             $(FRAMEWORK_DIR)/app.mk
  include             $(IAPWS95_DIR)/libSBTL.mk
endif

ifeq ($(ENABLE_NEK), yes)
  ADDITIONAL_CPPFLAGS += $(NEKRS_INCLUDES)
  libmesh_CXXFLAGS    += -DENABLE_NEK_COUPLING
endif

ifeq ($(ENABLE_OPENMC), yes)
  ADDITIONAL_CPPFLAGS += $(HDF5_INCLUDES) $(OPENMC_INCLUDES)
  libmesh_CXXFLAGS    += -DENABLE_OPENMC_COUPLING
endif

ifeq ($(ENABLE_DIABLO), yes)
  ADDITIONAL_CPPFLAGS += $(DIABLO_INCLUDES)
  libmesh_CXXFLAGS    += -DENABLE_DIABLO_COUPLING 
endif
ifeq ($(ENABLE_DAGMC), yes)
  libmesh_CXXFLAGS    += -DENABLE_DAGMC

  # this flag is used in OpenMC
  libmesh_CXXFLAGS    += -DDAGMC
endif

# ======================================================================================
# External apps
# ======================================================================================

# libmesh_CXX, etc, were defined in build.mk
export CXX := $(libmesh_CXX)
export CC  := $(libmesh_CC)
export FC  := $(libmesh_F90)
export FFLAGS := $(libmesh_FFLAGS)
export CFLAGS := $(libmesh_CFLAGS)
export CXXFLAGS := $(libmesh_CXXFLAGS)
export CPPFLAGS := $(libmesh_CPPFLAGS)
export LDFLAGS := $(libmesh_LDFLAGS)
export LIBS := $(libmesh_LIBS)

export CARDINAL_DIR

APPLICATION_DIR    := $(CARDINAL_DIR)
APPLICATION_NAME   := cardinal
BUILD_EXEC         := yes
GEN_REVISION       := no
DEP_APPS           := $(shell $(FRAMEWORK_DIR)/scripts/find_dep_apps.py $(APPLICATION_NAME))

ifeq ($(ENABLE_DAGMC), yes)
  ENABLE_DAGMC     := ON
  include          $(CARDINAL_DIR)/config/moab.mk
  include          $(CARDINAL_DIR)/config/dagmc.mk
else

build_dagmc:
	$(info Skipping DagMC build because ENABLE_DAGMC is not set to 'yes')

build_moab:
	$(info Skipping MOAB build because ENABLE_DAGMC is not set to 'yes')

endif

# autoconf-archive puts some arguments (e.g. -std=c++17) into the compiler
# variable rather than the compiler flags variable.
#
# cmake allows this, but wants any compiler arguments to be
# semicolon-separated, not space-separated
space := $(subst ,, )
LIBMESH_CC_LIST := $(subst $(space),;,$(libmesh_CC))
LIBMESH_CXX_LIST := $(subst $(space),;,$(libmesh_CXX))
LIBMESH_F90_LIST := $(subst $(space),;,$(libmesh_F90))

ifeq ($(ENABLE_NEK), yes)
  include            $(CARDINAL_DIR)/config/nekrs.mk
else

build_nekrs:
	$(info Skipping Nek build because ENABLE_NEK is not set to 'yes')

endif

ifeq ($(ENABLE_OPENMC), yes)
  include            $(CARDINAL_DIR)/config/openmc.mk
else

build_openmc:
	$(info Skipping OpenMC build because ENABLE_OPENMC is not set to 'yes')

endif

# ======================================================================================
# Building app objects defined in app.mk
# ======================================================================================

# ADDITIONAL_LIBS are used for linking in app.mk
# CC_LINKER_SLFLAG is from petscvariables
ADDITIONAL_LIBS := -L$(CARDINAL_DIR)/lib $(CC_LINKER_SLFLAG)$(CARDINAL_DIR)/lib

ifeq ($(ENABLE_NEK), yes)
  ADDITIONAL_LIBS += -L$(NEKRS_LIBDIR) -lnekrs -locca $(CC_LINKER_SLFLAG)$(NEKRS_LIBDIR)
endif

ifeq ($(ENABLE_OPENMC), yes)
  ADDITIONAL_LIBS += -L$(OPENMC_LIBDIR) -lopenmc -lhdf5_hl $(CC_LINKER_SLFLAG)$(OPENMC_LIBDIR)
endif

ifeq ($(ENABLE_DIABLO), yes)
  ADDITIONAL_LIBS += -Wl,-rpath,$(DIABLO_LIBDIR) -Wl,-rpath,$(DIABLO_EXTERNAL_LIBDIR)
endif

include            $(FRAMEWORK_DIR)/app.mk

# app_objects are defined in moose.mk and built according to the rules in build.mk
# We need to build these first so we get include dirs
$(app_objects): build_nekrs build_moab build_dagmc build_openmc
$(test_objects): build_nekrs build_moab build_dagmc build_openmc

CARDINAL_EXTERNAL_FLAGS := \
	-L$(CARDINAL_DIR)/lib \
	$(CC_LINKER_SLFLAG)$(CARDINAL_DIR)/lib \
	$(BLASLAPACK_LIB) \
	$(PETSC_EXTERNAL_LIB_BASIC)

ifeq ($(ENABLE_NEK), yes)
  CARDINAL_EXTERNAL_FLAGS += -L$(NEKRS_LIBDIR) -lnekrs $(CC_LINKER_SLFLAG)$(NEKRS_LIBDIR) 
endif

ifeq ($(ENABLE_OPENMC), yes)
  CARDINAL_EXTERNAL_FLAGS += -L$(OPENMC_LIBDIR) -L$(HDF5_LIBDIR) -lopenmc \
	                           $(CC_LINKER_SLFLAG)$(OPENMC_LIBDIR) \
	                           $(CC_LINKER_SLFLAG)$(HDF5_LIBDIR)
endif

ifeq ($(ENABLE_DIABLO), yes)
 CARDINAL_EXTERNAL_FLAGS += -L$(DIABLO_LIBDIR) -L$(DIABLO_EXTERNAL_LIBDIR) -l$(DIABLO_LIB) $(DIABLO_EXTRA_LIBS) $(CC_LINKER_SLFLAG)$(DIABLO_LIBDIR) $(CC_LINKER_SLFLAG)$(DIABLO_EXTERNAL_LIBDIR)
endif

# EXTERNAL_FLAGS are for rules in app.mk
$(app_LIB): EXTERNAL_FLAGS := $(CARDINAL_EXTERNAL_FLAGS)
$(app_test_LIB): EXTERNAL_FLAGS := $(CARDINAL_EXTERNAL_FLAGS)
$(app_EXEC): EXTERNAL_FLAGS := $(CARDINAL_EXTERNAL_FLAGS)
