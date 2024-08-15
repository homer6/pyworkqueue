cmake_minimum_required(VERSION 3.12)
project(pyworkqueue CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python
find_package(Python 3.10.13 REQUIRED COMPONENTS Interpreter Development)

# Find nanobind
execute_process(
    COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
    OUTPUT_VARIABLE NB_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT
)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)

# Add executable

include_directories(src)

# Add your source files here
set(SOURCES
    src/main.cc
)

add_executable(pyworkqueue ${SOURCES})


# Link against nanobind and Python
target_link_libraries(pyworkqueue PRIVATE nanobind::nanobind Python::Python)

# Include directories
target_include_directories(pyworkqueue PRIVATE ${Python_INCLUDE_DIRS})

# Set output directory
set_target_properties(pyworkqueue PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Ensure proper flags for embedding Python
if(UNIX)
    target_link_options(pyworkqueue PRIVATE "LINKER:--no-as-needed")
endif()

# Add Python's library directory to runtime path
if(UNIX AND NOT APPLE)
    set_target_properties(pyworkqueue PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "$ORIGIN:${Python_LIBRARY_DIRS}"
    )
endif()