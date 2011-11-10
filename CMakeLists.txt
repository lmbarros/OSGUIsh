#
# OSGUIsh
#

# General settings
project(OSGUIsh)
cmake_minimum_required(VERSION 2.6)
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "3")

find_package(OpenSceneGraph 2.8 REQUIRED
    COMPONENTS osgViewer osgUtil osgGA osgDB osgText)

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.39 REQUIRED COMPONENTS signals)
add_definitions(-DBOOST_ALL_DYN_LINK)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${Boost_INCLUDE_DIRS}
                    ${OPENSCENEGRAPH_INCLUDE_DIRS})

# Link directories
link_directories(${Boost_LIBRARY_DIRS})

# Enable warnings when compiling with G++
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif(CMAKE_COMPILER_IS_GNUCXX)

# Build the library
set(OSGUIshSources
    Sources/EventHandler.cpp
    Sources/FocusPolicy.cpp
    Sources/ManualFocusPolicy.cpp
    Sources/MouseDownFocusPolicy.cpp
    Sources/MouseOverFocusPolicy.cpp
    Sources/Types.cpp)

add_library(OSGUIsh STATIC ${OSGUIshSources})

target_link_libraries(OSGUIsh
    ${OPENSCENEGRAPH_LIBRARIES}
    ${Boost_LIBRARIES})

# Demos
add_executable(Simplest Demos/Simplest.cpp)
target_link_libraries(Simplest
    ${OPENSCENEGRAPH_LIBRARIES}
    ${Boost_LIBRARIES}
    OSGUIsh)
set_property(TARGET Simplest
    PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(ThreeObjects Demos/ThreeObjects.cpp)
target_link_libraries(ThreeObjects
    ${OPENSCENEGRAPH_LIBRARIES}
    ${Boost_LIBRARIES}
    OSGUIsh)
set_property(TARGET ThreeObjects
    PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(FocusPolicies Demos/FocusPolicies.cpp)
target_link_libraries(FocusPolicies
    ${OPENSCENEGRAPH_LIBRARIES}
    ${Boost_LIBRARIES}
    OSGUIsh)
set_property(TARGET FocusPolicies
    PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(HUD Demos/HUD.cpp)
target_link_libraries(HUD
    ${OPENSCENEGRAPH_LIBRARIES}
    ${Boost_LIBRARIES}
    OSGUIsh)
set_property(TARGET HUD
    PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_executable(PointsAndLines Demos/PointsAndLines.cpp)
target_link_libraries(PointsAndLines
    ${OPENSCENEGRAPH_LIBRARIES}
    ${Boost_LIBRARIES}
    OSGUIsh)
set_property(TARGET PointsAndLines
    PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Copies 'Data' to same place as the executable -- it's needed there
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Data
        ${PROJECT_BINARY_DIR}/Data)
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_SOURCE_DIR}/Data
        ${PROJECT_BINARY_DIR}/Data)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
