cmake_minimum_required(VERSION 3.12)
project(pyworkqueue CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python
find_package(Python 3.10.13 REQUIRED COMPONENTS Interpreter Development)

# Check if nanobind is installed
execute_process(
    COMMAND "${Python_EXECUTABLE}" -c "import nanobind; print(nanobind.__file__)"
    RESULT_VARIABLE NANOBIND_IMPORT_RESULT
    OUTPUT_VARIABLE NANOBIND_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(NOT NANOBIND_IMPORT_RESULT EQUAL 0)
    message(FATAL_ERROR "nanobind is not installed. Please install it using:\n"
                        "  ${Python_EXECUTABLE} -m pip install nanobind\n"
                        "Then, delete the CMakeCache.txt file and run CMake again.")
endif()

# Get nanobind include directory
get_filename_component(NANOBIND_DIR ${NANOBIND_PATH} DIRECTORY)
set(NANOBIND_INCLUDE_DIR "${NANOBIND_DIR}/include")

if(NOT EXISTS "${NANOBIND_INCLUDE_DIR}/nanobind/nanobind.h")
    message(FATAL_ERROR "Cannot find nanobind.h in ${NANOBIND_INCLUDE_DIR}")
endif()

# Add executable
include_directories(src ${NANOBIND_INCLUDE_DIR})

# Add your source files here
set(SOURCES
    src/PyWorkQueueApp.cc
    src/main.cc
)

add_executable(pyworkqueue ${SOURCES})

# Link against Python
target_link_libraries(pyworkqueue PRIVATE Python::Python)

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