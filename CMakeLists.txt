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
set( DDS_ROOT "/home/borotech/Dev/libs/OpenDDS-3.9" )
set( ACE_ROOT "/home/borotech/Dev/libs/OpenDDS-3.9/ACE_wrappers" )
set( MPC_ROOT "/home/borotech/Dev/libs/OpenDDS-3.9/ACE_wrappers/MPC" )
set( TAO_ROOT "/home/borotech/Dev/libs/OpenDDS-3.9/ACE_wrappers/TAO" )

include_directories( ${DDS_ROOT} )
include_directories( ${ACE_ROOT} )
include_directories( ${MPC_ROOT} )
include_directories( ${TAO_ROOT} )
include_directories( ${TAO_ROOT}/orbsvcs )


# Specify the directories where libraries are located.  	
file( GLOB OPENDDS_LIBS
						${DDS_ROOT}/dds/libOpenDDS_Dcps.so
						${ACE_ROOT}/lib/libTAO_AnyTypeCode.so
						${ACE_ROOT}/lib/libTAO_PortableServer.so
						${ACE_ROOT}/lib/libTAO.so
						${ACE_ROOT}/lib/libACE.so		
)

#file( GLOB_RECURSE ALL_DDS_LIBS ${DDS_ROOT}/*.so )

#message( STATUS "ALL_DDS_LIBS: " ${ALL_DDS_LIBS} )

message( STATUS "OPENDDS_LIBS: " ${OPENDDS_LIBS} )

# GLOB the IDL files for lib
file( GLOB IDL_LIB_H ${CMAKE_SOURCE_DIR}/OpenDDS/Source/Cpp/Generated/*.h )
file( GLOB IDL_LIB_CPP ${CMAKE_SOURCE_DIR}/OpenDDS/Source/Cpp/Generated/*.cpp )
file( GLOB IDL_LIB ${IDL_LIB_H} ${IDL_LIB_CPP} )

# IDL Generated Libs - Modify as you add/subtract IDLs
#add_library( IDL_LIBS_STATIC STATIC ${IDL_LIB} )

message( STATUS "IDL_LIB: " ${IDL_LIB} )

# Creates the executable file with the source files
add_executable( OpenDDS_SkeletonNodeApp 
					Source/main.cpp 
                    Source/CAppNode.cpp
                    Source/CAppNodeImpl.cpp
                    Source/Utility/CSignalException.cpp
)

message(STATUS "Source DIR:" ${CMAKE_SOURCE_DIR})

target_link_libraries( OpenDDS_SkeletonNodeApp 
#										${IDL_LIBS_STATIC}
										${OPENDDS_LIBS}
)

### PostBuild Copy Files ###
file( COPY Source/Config DESTINATION "${CMAKE_SOURCE_DIR}/build" )

message(STATUS "///////////////////////////////////////////////////////////////")
message(STATUS "/////////////// END CMAKE MESSAGES ////////////////////////////")
message(STATUS "///////////////////////////////////////////////////////////////")
