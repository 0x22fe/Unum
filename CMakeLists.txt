# Unum Project CMake File
cmake_minimum_required(VERSION 3.10.2)
include(ExternalProject)

project(Unum C)

# Refer to https://stackoverflow.com/a/47801116/4909645
function(make_includable input_file output_file)
    file(READ ${input_file} content)
    string(REPLACE "\n" "\\n" content ${content})
    string(REPLACE "\\\"" "\\\\\"" content ${content})
    string(REPLACE "\"" "\\\"" content ${content})
    set(new_data "\"${content}\"")
    file(WRITE ${output_file} "${new_data}")
endfunction(make_includable)

ExternalProject_Add(plibsys
  GIT_REPOSITORY https://github.com/saprykin/plibsys
  PREFIX ${CMAKE_SOURCE_DIR}/external/plibsys
  CMAKE_ARGS -DPLIBSYS_TESTS=OFF -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/plibsys
)

ExternalProject_Get_Property(plibsys BINARY_DIR)
ExternalProject_Get_Property(plibsys INSTALL_DIR)
ExternalProject_Get_Property(plibsys SOURCE_DIR)

# message(FATAL_ERROR "Src: ${SOURCE_DIR} and Install: ${INSTALL_DIR}")

add_executable(${PROJECT_NAME} src/Unum.c src/Main.c)

set(${PROJECT_NAME}_VERSION 1 0 0)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

# Generate Unum libraries
make_includable(src/libraries/bindings/Math.u src/libraries/generated/Math.h)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/plibsys/include/plibsys)

if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_BINARY_DIR}/plibsys/lib/libplibsys.dll)
endif()

if(APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -std=c99) #-Werror)
    target_link_libraries(${PROJECT_NAME} m ${CMAKE_BINARY_DIR}/plibsys/lib/libplibsys.dylib)
endif()

if(UNIX AND NOT APPLE)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -std=c99) #-Werror)
    target_link_libraries(${PROJECT_NAME} m ${CMAKE_BINARY_DIR}/plibsys/lib/libplibsys.so)
endif()
