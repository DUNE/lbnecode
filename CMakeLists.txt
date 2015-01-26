# ======================================================================
#  larsoft main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/larsoft/ups/setup_for_development <-d|-p>
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#  make
#  make test
#  make install
#  make package (builds distribution tarfile)
# ======================================================================
# LarSoft is an empty product
# It sets up all of the larsoft ups products (lardata, larsim, larreco, etc.)
# ======================================================================


# use cmake 2.8 or later
cmake_minimum_required (VERSION 2.8)

project(lbnecode)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (CETBUILDTOOLS_VERSION)

set(CMAKE_MODULE_PATH $ENV{ART_DIR}/Modules
		      $ENV{CETBUILDTOOLS_DIR}/Modules
		      ${CMAKE_MODULE_PATH})

include(CetCMakeEnv)
cet_cmake_env()

set_install_root()

cet_set_compiler_flags(DIAGS CAUTIOUS
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic -Wno-unused-local-typedefs
)

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_ups_product( larcore v1_00_00 )
find_ups_product( larevt v1_00_00 )
find_ups_product( lardata v1_00_00 )
find_ups_product( larsim v1_00_00 )
find_ups_product( larpandora v1_00_00 )
find_ups_product( larreco v1_00_00 )
find_ups_product( nutools v1_00_00 )
find_ups_product( art v1_09_00 )
find_ups_product( clhep v2_1_3_0 )
find_ups_product( messagefacility v1_10_00 )
find_ups_product( cetbuildtools v3_10_00 )
find_ups_product( postgresql v9_1_5 )
find_ups_product( artdaq_core v01_03_6 )

# macros for dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

# ADD SOURCE CODE SUBDIRECTORIES HERE
add_subdirectory(lbne)
# release database
add_subdirectory(releaseDB)
# subdirectory for test code
add_subdirectory(test)
# ups - table and config files
add_subdirectory(ups)
# subdirectory for fcl files
add_subdirectory(fcl)

# packaging utility
include(UseCPack)


# ======================================================================
# Everything below this is for an alternate cmake build
# ======================================================================

ELSE()

cmake_minimum_required(VERSION 2.8.12)

if(POLICY CMP0025)
	cmake_policy(SET CMP0025 OLD)
endif()

if(POLICY CMP0042)
	cmake_policy(SET CMP0042 NEW)
endif()

project(lbnecode)

set(lbnecode_VERSION "1.0.0")
set(lbnecode_VERSION_MAJOR 1)
set(lbnecode_VERSION_MINOR 0)
set(lbnecode_VERSION_PATCH 0)

set(lbnecode_SOVERSION "1.0.0")


set(lbnecode_DEBUG_POSTFIX "d")

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/Modules)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

set(BASE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/BuildProducts")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BASE_OUTPUT_DIRECTORY}/${CMAKE_INSTALL_LIBDIR}")

find_package(art 1.11.3 REQUIRED)

include(artTools)

find_package(FNALCore 0.1.0 REQUIRED)
find_package(nutools 1.07.00)
find_package( larcore 1.0.0 )
find_package( lardata 1.0.0 )
find_package( larevt 1.0.0 )
find_package( larsim 1.0.0 )
find_package( larreco 1.0.0 )
find_package( larpandora 1.0.0 )
find_package( artdaq-core 1.0.0 )

find_package( Boost 1.53.0 
     REQUIRED
      serialization
      date_time
     )

find_package(GCCXML 0.9.0 REQUIRED)

find_package(CLHEP 2.2.0.3 REQUIRED)

find_package(SQLite3 3.8.5 REQUIRED)

find_package(ROOT 5.34.20 REQUIRED
     Core
     Cint
     Cintex
     Hist
     Matrix
     Reflex
     RIO
     Thread
     Tree
)

if(NOT ROOT_python_FOUND)
     message(FATAL_ERROR "art requires ROOT with Python support")
endif()

find_package(TBB 4.1.0 REQUIRED)

#find_package( PostgreSQL 9.1.5 )

set(PQ ${PostgreSQL_LIBRARIES} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FNALCore_CXX_FLAGS} -O3 -g -DNDEBUG -fno-omit-frame-pointer -pedantic -Wno-unused-local-typedefs")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${art_INCLUDE_DIRS})
include_directories(${FNALCore_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${larcore_INCLUDE_DIRS})
include_directories(${lardata_INCLUDE_DIRS})
include_directories(${larevt_INCLUDE_DIRS})
include_directories(${larsim_INCLUDE_DIRS})
include_directories(${larreco_INCLUDE_DIRS})
include_directories(${larpandora_INCLUDE_DIRS})
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(${CLHEP_INCLUDE_DIRS})
include_directories(${Geant4_INCLUDE_DIRS})
include_directories( ${nutools_INCLUDE_DIRS} )
include_directories( ${PostgreSQL_INCLUDE_DIRS})
include_directories( ${GEANT4_FQ_DIR}/include )

# find_ups_product( artdaq_core v01_03_6 )
include_directories( ${artdaq-core_INCLUDE_DIRS} )

# macros for dictionary and simple_plugin
#include(ArtDictionary)
#include(ArtMake)
#include(BuildPlugins)

# ADD SOURCE CODE SUBDIRECTORIES HERE
add_subdirectory(lbne)
# release database
add_subdirectory(releaseDB)
# subdirectory for test code
add_subdirectory(test)
# ups - table and config files
add_subdirectory(ups)

configure_package_config_file(
  Modules/lbnecodeConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/lbnecodeConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lbnecode-${lbnecode_VERSION}
  PATH_VARS
    CMAKE_INSTALL_INCLUDEDIR
    CMAKE_INSTALL_LIBDIR
  )

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/lbnecodeConfigVersion.cmake
  VERSION ${lbnecode_VERSION}
  COMPATIBILITY AnyNewerVersion
  )

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/lbnecodeConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/lbnecodeConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lbnecode-${lbnecode_VERSION}
  COMPONENT Development
  )

install(EXPORT lbnecodeLibraries
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lbnecode-${lbnecode_VERSION}
  NAMESPACE lbnecode::
  COMPONENT Development
  )




# packaging utility
include(artCPack)

ENDIF()
