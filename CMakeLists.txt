cmake_minimum_required(VERSION 3.16)
project(engine)
#if (CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
#    add_compile_options(
#        -g
#        -O1
#        -fno-omit-frame-pointer
#        -fsanitize=address,undefined
#    )
#    add_link_options(
#        -fsanitize=address,undefined
#    )
#endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# Create your game executable target as usual
add_executable(prog ${SOURCES})

target_include_directories(prog PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Link to the actual SDL3 library.
target_link_libraries(prog PRIVATE m)
target_link_libraries(prog PRIVATE SDL3::SDL3)
