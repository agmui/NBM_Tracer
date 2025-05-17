cmake_minimum_required(VERSION 3.13)

# Enable debug symbols by default, must be done before project() statement
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif ()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

set(EXECUTABLE_NAME "tracer")
project(${EXECUTABLE_NAME} VERSION 1.0
        DESCRIPTION "Simple Raytracer"
        LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

option(SFML_WINDOW "builds with realtime raytrace SFML window" OFF)
if(SFML_WINDOW)
    add_definitions(-DSFML_WINDOW)
endif()

if (OBJ_DEBUG)
    add_definitions(-DOBJ_DEBUG=1)
endif (OBJ_DEBUG)
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions(-DRELEASE)
endif ()

add_definitions(-DEXEC_NAME="${EXECUTABLE_NAME}")

############################################################
# Create a library
############################################################
# Add interface library for the header-only library
#TODO: make this a header-only lib for release and static for debug
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp" "include/*.cpp" "include/*.c")
add_library(HeaderOnlyLib STATIC ${HEADERS})
target_include_directories(HeaderOnlyLib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/
)
set_target_properties(HeaderOnlyLib PROPERTIES LINKER_LANGUAGE CXX)
#message(CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

############################################################
# Create an executable
############################################################
file(GLOB_RECURSE SOURCES "src/**.h" "src/**.hpp" "src/**.cpp" "src/**.c")
file(GLOB_RECURSE RESOURCES "resources/")

# Add an executable with the above sources
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${RESOURCES})

# link the new hello_library target with the hello_binary target
target_link_libraries(${PROJECT_NAME} PRIVATE HeaderOnlyLib)
target_include_directories(${PROJECT_NAME}
#        PRIVATE
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        # this lets me include files relative to the root source directory with a <> pair
        ${PROJECT_SOURCE_DIR}/src/main
        ${CMAKE_CURRENT_SOURCE_DIR}
)

#target_compile_definitions(${PROJECT_NAME} PRIVATE OBJ_DEBUG)

############################################################
# Adding GLFW
############################################################
if(SFML_WINDOW)
    include(FetchContent)
    FetchContent_Declare(SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 3.0.0
            GIT_SHALLOW ON
            EXCLUDE_FROM_ALL
            SYSTEM)
    FetchContent_MakeAvailable(SFML)
    target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics)
endif(SFML_WINDOW)

#find_package(SDL2 REQUIRED)
#target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
#target_link_libraries(
#        ${PROJECT_NAME}
#        PUBLIC
#        ${SDL2_LIBRARIES}
#)

#find_package(OpenGL REQUIRED)
#include_directories( ${OPENGL_INCLUDE_DIRS} )
#
#set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
#set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
#set( GLAD_GL "" )
#add_subdirectory(libs/glfw-3.4 build)
#
#target_include_directories(${PROJECT_NAME} PUBLIC  libs/glfw-3.4/include)
#target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES} glfw)

############################################################
# Set executable flags
############################################################
target_compile_options(${PROJECT_NAME} PUBLIC
        #        -Wall
        #        -fsanitize=address,undefined
        -Wfloat-conversion
)
      


#-fno-omit-frame-pointer #To get nicer stack traces in error messages add
# ASan flags: https://github.com/google/sanitizers/wiki/AddressSanitizerFlags

#set(CMAKE_CXX_FLAGS "-O3 -no-pie -march=native -ggdb -Wall \
#-Wextra -Wcast-align -Wdisabled-optimization -Wformat=2 \
#-Winit-self -Wmissing-include-dirs -Woverloaded-virtual \
#-Wshadow -Wsign-promo -Werror")
#
#set(CMAKE_CXX_FLAGS "-O3  -ggdb -std=gnu99 -I.")

#TODO: compile to specific cpu
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g3 -DDEBUG" CACHE STRING "Debug mode flags" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -Ofast -march=native" CACHE STRING "Release mode flags" FORCE)

if (UNIX)
    #    set( CMAKE_CXX_FLAGS_DEBUG_INIT "-fstack-protector" )
    #    set( CMAKE_CXX_FLAGS_DEBUG "-fstack-protector" )
    #    add_compile_options("DEBUG:-Og3 -Wall")
elseif (WIN32)
    #    set( CMAKE_CXX_FLAGS_DEBUG_INIT "/GS" )
endif ()


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

    message("using inter procedural optimization")
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result)
    if (result)
        set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif ()
endif ()


# this copies all resource files in the build directory
# we need this, because we want to work with paths relative to the executable
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})
#file(COPY ${RESOURCES} DESTINATION resources)

if (UNIX AND NOT APPLE)
    set(LIBS ${LIBS} ${CMAKE_DL_LIBS})
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})

