CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

IF(WIN32)
   SET(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory")
   MESSAGE(STATUS "WIN32")
ELSE()
   SET(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation Directory")
ENDIF()

PROJECT(Boost-Sender)

SET (EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin CACHE INTERNAL
  "Where to put the executables")

SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib CACHE INTERNAL
  "Where to put the libraries")
 
IF(WIN32)
   SET (VAC_DEBUG_PREFIX "d") 
ELSE()
   SET (VAC_DEBUG_PREFIX "") 
ENDIF()

# Setup the path to load our macros
SET (CMAKE_MODULE_PATH "${UCLTK_BINARY_DIR}/CMakeModules")

# Open the property setting
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET_PROPERTY(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

ADD_DEFINITIONS(-DQXT_STATIC)
ADD_DEFINITIONS(-DQXT_DOXYGEN_RUN)

if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
   cmake_policy(SET CMP0015 NEW)
endif(COMMAND cmake_policy)

FIND_PACKAGE(Boost)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS})
ELSE(Boost_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build without boost. Please set BOOST_DIR.")
ENDIF(Boost_FOUND)

# Generate the executable application for the current poject
ADD_EXECUTABLE (sender 
				sender.cpp
#				${FOO_MOC_H}
)

# Add the link-libraries for the current poject
TARGET_LINK_LIBRARIES (sender 
					   
					   
)
# The installation path for the application
INSTALL_TARGETS (/bin sender )


