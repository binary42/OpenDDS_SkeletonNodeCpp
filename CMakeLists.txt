cmake_minimum_required(VERSION 2.8)
project( OpenDDS_SkeletonNode )

set(TEMP_CXX_FLAGS  ${CMAKE_CXX_FLAGS} )
set(CMAKE_CXX_FLAGS "-std=c++11 -O3 ${TEMP_CXX_FLAGS}")  # TODO: Up the optimization level
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

message(STATUS "")
message(STATUS "///////////////////////////////////////////////////////////////")
message(STATUS "/////////////// CMAKE MESSAGES ////////////////////////////////")
message(STATUS "///////////////////////////////////////////////////////////////")

message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Linker flags:   ${CMAKE_EXE_LINKER_FLAGS}")

### including project headers ###
include_directories( ${CMAKE_SOURCE_DIR}/ThirdPartyLibs )
include_directories( ${CMAKE_SOURCE_DIR}/Source )
include_directories( ${CMAKE_SOURCE_DIR}/Source/Utility )
include_directories( ${CMAKE_SOURCE_DIR}/OpenDDS/Source/Cpp/Generated )

### ACE/TAO on your system -- Modify as needed -- ###
set( HOME "/home/binary9" )

set( DDS_ROOT "${HOME}/dev/OpenDDS-3.12" )
set( ACE_ROOT "${HOME}/dev/OpenDDS-3.12/ACE_wrappers" )
set( MPC_ROOT "${HOME}/dev/OpenDDS-3.12/ACE_wrappers/MPC" )
set( TAO_ROOT "${HOME}/dev/OpenDDS-3.12/ACE_wrappers/TAO" )

include_directories( ${DDS_ROOT} )
include_directories( ${DDS_ROOT}/dds )
include_directories( ${ACE_ROOT} )
include_directories( ${ACE_ROOT}/ace )
include_directories( ${MPC_ROOT} )
include_directories( ${TAO_ROOT} )
include_directories( ${TAO_ROOT}/orbsvcs )


# Specify the directories where libraries are located.  	
file( GLOB OPENDDS_LIBS
						${DDS_ROOT}/dds/*.so
						${ACE_ROOT}/lib/*.so
						#${DDS_ROOT}/lib/*.so
)

message( STATUS "OPENDDS_LIBS: " ${OPENDDS_LIBS} )

# Grab the generated idl related files
file( GLOB IDL_CPP_FILES ${CMAKE_SOURCE_DIR}/OpenDDS/Source/Cpp/Generated/*.cpp )

# Grab the main cpp files
file( GLOB SOURCE_CPP_FILES ${CMAKE_SOURCE_DIR}/Source/*.cpp )

# Grab utils
file( GLOB UTILS_CPP_FILES ${CMAKE_SOURCE_DIR}/Source/Utility/*.cpp )

# Creates the executable file with the source files
add_executable( OpenDDS_SkeletonNodeApp 
					# IDL CPPs
					${IDL_CPP_FILES}
					
					# Node Specific CPPs
					${SOURCE_CPP_FILES}
					${UTILS_CPP_FILES}
)

message(STATUS "Source DIR:" ${CMAKE_SOURCE_DIR})

target_link_libraries( OpenDDS_SkeletonNodeApp 
									${OPENDDS_LIBS}
									${DDS_ROOT}/dds/libOpenDDS_Dcps.so
									${ACE_ROOT}/lib/libTAO_AnyTypeCode.so
									${ACE_ROOT}/lib/libTAO.so
									${ACE_ROOT}/lib/libACE.so
)

### PostBuild Copy Files ###
file( COPY Source/Config DESTINATION "${CMAKE_SOURCE_DIR}/build" )

message(STATUS "///////////////////////////////////////////////////////////////")
message(STATUS "/////////////// END CMAKE MESSAGES ////////////////////////////")
message(STATUS "///////////////////////////////////////////////////////////////")
