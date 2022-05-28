# CMAKE-VERSION
cmake_minimum_required(VERSION 3.21)

# PROJECT DEFS
project(pictel_sound)
#
#file(MAKE_DIRECTORY ${RESULT_LIB_OUTPUT_DIR})

#macro(print_all_variables)
#    message(STATUS "print_all_variables------------------------------------------{")
#    get_cmake_property(_variableNames VARIABLES)
#    foreach (_variableName ${_variableNames})
#        message(STATUS "${_variableName}=${${_variableName}}")
#    endforeach()
#    message(STATUS "print_all_variables------------------------------------------}")
#endmacro()
#
#print_all_variables()

# FLAGS
set(CMAKE_CXX_STANDARD 14)
set(RESULT_LIB_OUTPUT_DIR ${PROJECT_BINARY_DIR}/build)

# INCLUDES
include_directories("external/ogg-vorbis/include")

# TARGET-MACOS LIB
add_library(pictel_sound-macos
        src/pictel-sound-lib/library.cpp
        src/pictel-sound-lib/pictel-sound.cpp src/pictel-sound-lib/include/pictel-sound.h)
add_custom_command(
        TARGET pictel_sound-macos POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RESULT_LIB_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RESULT_LIB_OUTPUT_DIR}/include
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/src/pictel-sound-lib/include ${RESULT_LIB_OUTPUT_DIR}/include
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/libpictel_sound-macos.a ${RESULT_LIB_OUTPUT_DIR}
        VERBATIM
)

# TARGET-MACOS SAMPLE
add_executable(pictel_sound-macos-sample src/pictel-sound-sample/macos/main.cpp)
add_dependencies(pictel_sound-macos-sample pictel_sound-macos)
target_include_directories(pictel_sound-macos-sample PUBLIC
        ${PROJECT_SOURCE_DIR}/src/pictel-sound-lib/include
)
target_link_libraries(pictel_sound-macos-sample PUBLIC stdc++
        "-framework Foundation"
        "-framework CoreFoundation"
        pictel_sound-macos
        objc)