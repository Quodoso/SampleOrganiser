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
CC=gcc-8
CCC=g++-8
CXX=g++-8
FC=gfortran-8
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AudioFeature.o \
	${OBJECTDIR}/AudioFile.o \
	${OBJECTDIR}/CLIHandler.o \
	${OBJECTDIR}/Json_Handler.o \
	${OBJECTDIR}/distanceMetric.o \
	${OBJECTDIR}/essentiaInterface.o \
	${OBJECTDIR}/featureSelektor.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/printObjects.o \
	${OBJECTDIR}/simpleTests.o \
	${OBJECTDIR}/src/hierarchCluster/fastcluster.o \
	${OBJECTDIR}/statInterface.o \
	${OBJECTDIR}/utilities.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/featureTest.o

# C Compiler Flags
CFLAGS=-03

# CC Compiler Flags
CCFLAGS=-m64 -O3 -s -DNDEBUG -fopenmp -D_GLIBCXX_PARALLEL -funsafe-loop-optimizations -fgraphite-identity -floop-parallelize-all
CXXFLAGS=-m64 -O3 -s -DNDEBUG -fopenmp -D_GLIBCXX_PARALLEL -funsafe-loop-optimizations -fgraphite-identity -floop-parallelize-all

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lstdc++fs `pkg-config --libs essentia` `pkg-config --libs fftw3` `pkg-config --libs fftw3f` `pkg-config --libs libavformat` `pkg-config --libs libavcodec` `pkg-config --libs libavutil` `pkg-config --libs samplerate` `pkg-config --libs libchromaprint` `pkg-config --libs gsl` `pkg-config --libs libgvc` `pkg-config --libs libconfig++` `pkg-config --libs mpi-cxx` `pkg-config --libs yaml-0.1` `pkg-config --libs taglib`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sampleorganiser

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sampleorganiser: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/sampleorganiser ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/AudioFeature.o: AudioFeature.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AudioFeature.o AudioFeature.cpp

${OBJECTDIR}/AudioFile.o: AudioFile.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AudioFile.o AudioFile.cpp

${OBJECTDIR}/CLIHandler.o: CLIHandler.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CLIHandler.o CLIHandler.cpp

${OBJECTDIR}/Json_Handler.o: Json_Handler.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Json_Handler.o Json_Handler.cpp

${OBJECTDIR}/distanceMetric.o: distanceMetric.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/distanceMetric.o distanceMetric.cpp

${OBJECTDIR}/essentiaInterface.o: essentiaInterface.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/essentiaInterface.o essentiaInterface.cpp

${OBJECTDIR}/featureSelektor.o: featureSelektor.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/featureSelektor.o featureSelektor.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/printObjects.o: printObjects.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/printObjects.o printObjects.cpp

${OBJECTDIR}/simpleTests.o: simpleTests.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/simpleTests.o simpleTests.cpp

${OBJECTDIR}/src/hierarchCluster/fastcluster.o: src/hierarchCluster/fastcluster.cpp
	${MKDIR} -p ${OBJECTDIR}/src/hierarchCluster
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/hierarchCluster/fastcluster.o src/hierarchCluster/fastcluster.cpp

${OBJECTDIR}/statInterface.o: statInterface.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/statInterface.o statInterface.cpp

${OBJECTDIR}/utilities.o: utilities.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utilities.o utilities.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/featureTest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   


${TESTDIR}/tests/featureTest.o: tests/featureTest.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils -I. `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/featureTest.o tests/featureTest.cpp


${OBJECTDIR}/AudioFeature_nomain.o: ${OBJECTDIR}/AudioFeature.o AudioFeature.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/AudioFeature.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AudioFeature_nomain.o AudioFeature.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/AudioFeature.o ${OBJECTDIR}/AudioFeature_nomain.o;\
	fi

${OBJECTDIR}/AudioFile_nomain.o: ${OBJECTDIR}/AudioFile.o AudioFile.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/AudioFile.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AudioFile_nomain.o AudioFile.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/AudioFile.o ${OBJECTDIR}/AudioFile_nomain.o;\
	fi

${OBJECTDIR}/CLIHandler_nomain.o: ${OBJECTDIR}/CLIHandler.o CLIHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/CLIHandler.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CLIHandler_nomain.o CLIHandler.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/CLIHandler.o ${OBJECTDIR}/CLIHandler_nomain.o;\
	fi

${OBJECTDIR}/Json_Handler_nomain.o: ${OBJECTDIR}/Json_Handler.o Json_Handler.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/Json_Handler.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Json_Handler_nomain.o Json_Handler.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/Json_Handler.o ${OBJECTDIR}/Json_Handler_nomain.o;\
	fi

${OBJECTDIR}/distanceMetric_nomain.o: ${OBJECTDIR}/distanceMetric.o distanceMetric.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/distanceMetric.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/distanceMetric_nomain.o distanceMetric.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/distanceMetric.o ${OBJECTDIR}/distanceMetric_nomain.o;\
	fi

${OBJECTDIR}/essentiaInterface_nomain.o: ${OBJECTDIR}/essentiaInterface.o essentiaInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/essentiaInterface.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/essentiaInterface_nomain.o essentiaInterface.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/essentiaInterface.o ${OBJECTDIR}/essentiaInterface_nomain.o;\
	fi

${OBJECTDIR}/featureSelektor_nomain.o: ${OBJECTDIR}/featureSelektor.o featureSelektor.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/featureSelektor.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/featureSelektor_nomain.o featureSelektor.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/featureSelektor.o ${OBJECTDIR}/featureSelektor_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

${OBJECTDIR}/printObjects_nomain.o: ${OBJECTDIR}/printObjects.o printObjects.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/printObjects.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/printObjects_nomain.o printObjects.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/printObjects.o ${OBJECTDIR}/printObjects_nomain.o;\
	fi

${OBJECTDIR}/simpleTests_nomain.o: ${OBJECTDIR}/simpleTests.o simpleTests.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/simpleTests.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/simpleTests_nomain.o simpleTests.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/simpleTests.o ${OBJECTDIR}/simpleTests_nomain.o;\
	fi

${OBJECTDIR}/src/hierarchCluster/fastcluster_nomain.o: ${OBJECTDIR}/src/hierarchCluster/fastcluster.o src/hierarchCluster/fastcluster.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/hierarchCluster
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/hierarchCluster/fastcluster.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/hierarchCluster/fastcluster_nomain.o src/hierarchCluster/fastcluster.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/hierarchCluster/fastcluster.o ${OBJECTDIR}/src/hierarchCluster/fastcluster_nomain.o;\
	fi

${OBJECTDIR}/statInterface_nomain.o: ${OBJECTDIR}/statInterface.o statInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/statInterface.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/statInterface_nomain.o statInterface.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/statInterface.o ${OBJECTDIR}/statInterface_nomain.o;\
	fi

${OBJECTDIR}/utilities_nomain.o: ${OBJECTDIR}/utilities.o utilities.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/utilities.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O3 -I/usr/local/include/essentia -I/usr/local/include/essentia/scheduler -I/usr/local/include/essentia/streaming -I/usr/local/include/essentia/utils `pkg-config --cflags essentia` `pkg-config --cflags fftw3` `pkg-config --cflags fftw3f` `pkg-config --cflags libavformat` `pkg-config --cflags libavcodec` `pkg-config --cflags libavutil` `pkg-config --cflags samplerate` `pkg-config --cflags libchromaprint` `pkg-config --cflags gsl` `pkg-config --cflags libgvc` `pkg-config --cflags libconfig++` `pkg-config --cflags mpi-cxx` `pkg-config --cflags yaml-0.1` `pkg-config --cflags taglib` -std=c++17  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/utilities_nomain.o utilities.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/utilities.o ${OBJECTDIR}/utilities_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
