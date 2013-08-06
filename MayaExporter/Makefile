#-
# ==========================================================================
# Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk
# license agreement provided at the time of installation or download,
# or which otherwise accompanies this software in either electronic
# or hard copy form.
# ==========================================================================
#+

# NOTE: MAYA_LOCATION on Mac OS X points to Maya.app/Contents

# Change location if you have a non standard install.
DEVKIT_LOCATION = $(MAYA_LOCATION)/../../devkit

# Make sure this is in your path.
C++			= g++

# By default, we will try to build a universal binary to match the same
# architectures as the installed Maya binary. If you only want one specific
# architecture (e.g i386 or x86_64) then set it below.
PREFERRED_ARCHITECTURE	= 

# Determine the architectures to build.
MAYABIN = ${MAYA_LOCATION}/bin/maya
#MAYA_ARCHES = $(shell lipo -info $(MAYABIN) | sed 's/^.*://')

ifneq ($(PREFERRED_ARCHITECTURE),)
	MAYA_ARCHES = $(filter $(PREFERRED_ARCHITECTURE),$(MAYA_ARCHES))

	ifeq ($(MAYA_ARCHES),)
		$(error $(MAYABIN) does not support the '$(PREFERRED_ARCHITECTURE)' architecture.)
	endif
endif

ARCH_FLAGS = $(patsubst %,-arch %,$(MAYA_ARCHES))
CFLAGS	= -DAW_NEW_IOSTREAMS -DCC_GNU_ -DOSMac_ -DOSMacOSX_ \
		-DOSMac_MachO_ -DREQUIRE_IOSTREAM -fno-gnu-keywords -fpascal-strings -O3 \
		$(ARCH_FLAGS) \
		-include $(MAYA_LOCATION)/../../devkit/include/maya/OpenMayaMac.h \
		-I/Applications/Autodesk/maya2012/devkit/include/maya \
		-I/Applications/Autodesk/maya2012/devkit/include
		
C++FLAGS	= $(CFLAGS)
INCLUDES	= -I. -I$(MAYA_LOCATION)/../../devkit/include
LD		= $(MAYA_LOCATION)/../../devkit/bin/mayald MAYA_ARCHES="x86_64" MAYA_LOCATION="$(MAYA_LOCATION)"
TARGETS		= Exporter

.SUFFIXES: .cpp .cc .o .c

.cc.o:
	$(C++) -c $(C++FLAGS) $(INCLUDES) $<

.cpp.o:
	$(C++) -c $(C++FLAGS) $(INCLUDES) $<

all: $(TARGETS)

Exporter: AnimationExport.o \
BotLauncherExport.o \
CameraExport.o \
CollisionMeshExport.o \
CurveExport.o \
DoorsExport.o \
Exporter.o \
GeometryExport.o \
Globals.o \
GraphExport.o \
LocatorExport.o \
MaterialData.o \
MaterialExport.o \
MeshData.o \
ModelExporter.o \
PVRTTriStrip.o \
PhysicsExport.o \
RefSceneExport.o \
Saver.o \
SkinExport.o \
StopCircleExport.o \
TextureData.o \
Transform.o \
TransformData.o \
TransformExport.o \
UVAnimationExport.o \
Util.o \
WireExport.o \
stdafx.o

	$(LD) -o $@ $@.o



clean:
	rm -f *.o

Clean:
	rm -f $(TARGETS) *.o *.bak
