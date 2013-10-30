################################################################
# Project GUACAMOLE-EXAMPLES
################################################################

PROJECT(GUACAMOLE-EXAMPLES CXX)

# version number
set(GUACAMOLE_MAJOR 0)
set(GUACAMOLE_MINOR 1)
set(GUACAMOLE_PATCH 0)
set(GUACAMOLE_VERSION ${GUACAMOLE_MAJOR}.${GUACAMOLE_MINOR}.${GUACAMOLE_PATCH})
set(GUACAMOLE_DESCRIPTION "GUACAMOLE - an astonishing virtual reality engine")
set(GUACAMOLE_HOMEPAGE "http://www.GUACAMOLE.org")
set(GUACAMOLE_EXENAME "GUACAMOLE")
set(GUACAMOLE_PACKAGENAME "GUACAMOLE")

# We require at least version 2.8.0
cmake_minimum_required(VERSION 2.8.0)
SET(GUA_LIBRARY_DIRS "NOT FOUND" CACHE PATH "description")
SET(GUA_INCLUDE_DIRS "NOT FOUND" CACHE PATH "description")

if (UNIX)
	find_package(PkgConfig)
endif(UNIX)

# Location where cmake first looks for modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

################################################################
# Configure and find libraries
################################################################

include(find_compiler)
include(find_guacamole)

include(define_macros)

include(find_schism)
include(find_boost)
include(find_bullet)

if (UNIX)
  pkg_check_modules(ASSIMP REQUIRED assimp)
  pkg_check_modules(JSON REQUIRED jsoncpp)
elseif (WIN32)
  include(find_assimp)
endif (UNIX)

include(find_json)
include(find_cuda)

set(LIB_PATH
    ${SCHISM_LIBRARY_DIRS}
    ${BOOST_LIBRARY_DIRS}
    #${EV_LIBRARY_DIRS}
    #${ZMQ_LIBRARY_DIRS}
    #${PROTOBUF_LIBRARY_DIRS}
    ${BULLET_LIBRARY_DIRS}
    ${GUA_LIBRARY_DIRS}
	${ASSIMP_LIBRARY_DIRS}
	${JSON_LIBRARY_DIRS}
	${CUDA_LIBRARY_DIRS}
)
link_directories(${LIB_PATH})

set(LIBS
    ${SCHISM_LIBRARIES}
    ${BOOST_LIBRARIES}
    #${EV_LIBRARIES}
    #${ZMQ_LIBRARIES}
    #${PROTOBUF_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${GUA_LIBRARIES}
	${ASSIMP_LIBRARIES}
	${JSON_LIBRARIES}
	${CUDA_LIBRARIES}
)
message(${LIBS})
#link_libraries(${LIBS})

set(INCLUDE_PATHS
    ${SCHISM_INCLUDE_DIRS}
    ${BOOST_INCLUDE_DIRS}
    #${EV_INCLUDE_DIRS}
    #${ZMQ_INCLUDE_DIRS}
    #${PROTOBUF_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    ${GUA_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
	${JSON_INCLUDE_DIRS}
)
include_directories(${INCLUDE_PATHS})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

IF (UNIX)
	set(CMAKE_CXX_FLAGS_RELEASE "-s -O4 --std=c++0x -fpermissive")
	set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall --std=c++0x -fpermissive")
ELSEIF(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "-D _SECURE_SCL=0 -D _SCL_SECURE_NO_WARNINGS -D _CRT_SECURE_NO_DEPRECATE /MD /MP")
	set(CMAKE_CXX_FLAGS_DEBUG "-D _DEBUG /MDd /Zi")
ENDIF(UNIX)
	
################################################################
# Create examples
################################################################

file(GLOB _EXAMPLES ${CMAKE_CURRENT_SOURCE_DIR}/examples/*)
FOREACH(_EXAMPLE ${_EXAMPLES})
    add_subdirectory(${_EXAMPLE})
ENDFOREACH(_EXAMPLE ${_EXAMPLES})

################################################################
# Summary
################################################################

message( "" )
message( "Summary:" )
message( " build type: ${CMAKE_BUILD_TYPE}" )
message( "" )
message( " schism:" )
message( "   library: ${SCHISM_LIBRARIES}" )
message( "   lib path: ${SCHISM_LIBRARY_DIRS}" )
message( "   include: ${SCHISM_INCLUDE_DIRS}" )
message( "" )
message( " boost:" )
message( "   library: ${BOOST_LIBRARIES}" )
message( "   lib path: ${BOOST_LIBRARY_DIRS}" )
message( "   include: ${BOOST_INCLUDE_DIRS}" )
message( "" )
message( " bullet:" )
message( "   library: ${BULLET_LIBRARIES}" )
message( "   lib path: ${BULLET_LIBRARY_DIRS}" )
message( "   include: ${BULLET_INCLUDE_DIRS}" )
message( "" )
message( " guacamole:" )
message( "   library: ${GUA_LIBRARIES}" )
message( "   lib path: ${GUA_LIBRARY_DIRS}" )
message( "   include: ${GUA_INCLUDE_DIRS}" )
message( "" )
