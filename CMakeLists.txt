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
link_directories( ${LD_LIBRARY_PATH} )	

# Creates the executable file with the source files
# TODO: Possibly use GLOB to specify source files
add_executable( OpenDDS_SkeletonNodeApp 
					Source/main.cpp 
                    Source/CAppNode.cpp
                    Source/CAppNodeImpl.cpp
                    Source/Utility/CSignalException.cpp
)

message(STATUS "Source DIR:" ${CMAKE_SOURCE_DIR})

target_link_libraries( OpenDDS_SkeletonNodeApp 
										pthread
										OpenDDS_Dcps
										TAO_AnyTypeCode
										TAO
										ACE
)

### PostBuild Copy Files ###
file( COPY Source/Config DESTINATION "${CMAKE_SOURCE_DIR}/build" )

message(STATUS "///////////////////////////////////////////////////////////////")
message(STATUS "/////////////// END CMAKE MESSAGES ////////////////////////////")
message(STATUS "///////////////////////////////////////////////////////////////")
