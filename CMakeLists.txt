cmake_minimum_required(VERSION 3.25)
project(NBM_Tracer)

set(CMAKE_CXX_STANDARD 20)

############################################################
# Create a library
############################################################
# Add interface library for the header-only library
#file(GLOB_RECURSE LIB_HEADERS "lib/*.h" "lib/*.hpp" "lib/*.cpp" "lib/*.c")
#add_library(RayTracerLib STATIC ${LIB_HEADERS})
#target_include_directories(RayTracerLib
#        PUBLIC
#        ${CMAKE_SOURCE_DIR}/lib/
#)
#set_target_properties(RayTracerLib PROPERTIES LINKER_LANGUAGE CXX)
add_subdirectory(lib/muian_raytracer)

file(GLOB_RECURSE SRC_HEADERS "src/*.h" "src/*.hpp" "src/*.cpp" "src/*.c")
add_library(NetworkTracerLib STATIC ${SRC_HEADERS})
target_link_libraries(NetworkTracerLib PRIVATE tracer)
target_include_directories(NetworkTracerLib
        PUBLIC
        ${PROJECT_SOURCE_DIR}/lib
        ${CMAKE_SOURCE_DIR}/src/
)
set_target_properties(NetworkTracerLib PROPERTIES LINKER_LANGUAGE CXX)

############################################################
# Create an executable
############################################################

add_executable(NBM_Tracer_Client app/client_app.cpp)
add_executable(NBM_Tracer_Server app/server_app.cpp)

target_link_libraries(NBM_Tracer_Client PRIVATE NetworkTracerLib)
target_link_libraries(NBM_Tracer_Client PRIVATE tracer) # TODO: may not be needed
target_include_directories(NBM_Tracer_Client
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        PUBLIC
        # this lets me include files relative to the root source directory with a <> pair
#        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/lib
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(NBM_Tracer_Server PRIVATE NetworkTracerLib)
target_link_libraries(NBM_Tracer_Server PRIVATE tracer)
target_include_directories(NBM_Tracer_Server
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        PUBLIC
        # this lets me include files relative to the root source directory with a <> pair
        #        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/lib
        ${CMAKE_SOURCE_DIR}
)
############################################################
# Set executable flags
############################################################
target_compile_options(NBM_Tracer_Client PUBLIC
        #        -Wall
#                -fsanitize=address,undefined
#        -Wfloat-conversion
)

target_compile_options(NBM_Tracer_Server PUBLIC
        #        -Wall
#        -fsanitize=address,undefined
        #        -Wfloat-conversion
)


