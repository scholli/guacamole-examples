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

# Location where cmake first looks for modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

################################################################
# Configure and find libraries
################################################################

include(find_schism)
include(find_boost)
include(find_ev)
include(find_zmq)
include(find_protobuf)
include(find_bullet)
include(find_guacamole)

set(LIBS
    ${SCHISM_LIBRARIES}
    ${BOOST_LIBRARIES}
    ${EV_LIBRARIES}
    ${ZMQ_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${GUA_LIBRARIES}
)
link_libraries(${LIBS})

set(INCLUDE_PATHS
    ${SCHISM_INCLUDE_DIRS}
    ${BOOST_INCLUDE_DIRS}
    ${EV_INCLUDE_DIRS}
    ${ZMQ_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    ${GUA_INCLUDE_DIRS}
)
include_directories(${INCLUDE_PATHS})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-s -O4 --std=c++0x -fpermissive")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall --std=c++0x -fpermissive")

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
message( "   include: ${SCHISM_INCLUDE_DIRS}" )
message( "" )
message( " boost:" )
message( "   library: ${BOOST_LIBRARIES}" )
message( "   include: ${BOOST_INCLUDE_DIRS}" )
message( "" )
message( " ev:" )
message( "   library: ${EV_LIBRARIES}" )
message( "   include: ${EV_INCLUDE_DIRS}" )
message( "" )
message( " zmq:" )
message( "   library: ${ZMQ_LIBRARIES}" )
message( "   include: ${ZMQ_INCLUDE_DIRS}" )
message( "" )
message( " protobuf:" )
message( "   library: ${PROTOBUF_LIBRARIES}" )
message( "   include: ${PROTOBUF_INCLUDE_DIRS}" )
message( "" )
message( " bullet:" )
message( "   library: ${BULLET_LIBRARIES}" )
message( "   include: ${BULLET_INCLUDE_DIRS}" )
message( "" )
message( " guacamole:" )
message( "   library: ${GUA_LIBRARIES}" )
message( "   include: ${GUA_INCLUDE_DIRS}" )
message( "" )
