#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/glew_backend.o \
	${OBJECTDIR}/glut_backend.o \
	${OBJECTDIR}/math_3d.o \
	${OBJECTDIR}/ogldev_texture.o \
	${OBJECTDIR}/ogldev_util.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.a

${OBJECTDIR}/glew_backend.o: glew_backend.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../Include -I../Include/assimp -I../Include/ImageMagick-6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/glew_backend.o glew_backend.cpp

${OBJECTDIR}/glut_backend.o: glut_backend.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../Include -I../Include/assimp -I../Include/ImageMagick-6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/glut_backend.o glut_backend.cpp

${OBJECTDIR}/math_3d.o: math_3d.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../Include -I../Include/assimp -I../Include/ImageMagick-6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/math_3d.o math_3d.cpp

${OBJECTDIR}/ogldev_texture.o: ogldev_texture.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../Include -I../Include/assimp -I../Include/ImageMagick-6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ogldev_texture.o ogldev_texture.cpp

${OBJECTDIR}/ogldev_util.o: ogldev_util.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I../Include -I../Include/assimp -I../Include/ImageMagick-6 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ogldev_util.o ogldev_util.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
