cmake_minimum_required(VERSION 3.16)
project(SP_Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Output executable name ──────────────────────────────────────────────────
set(EXE_NAME FeedingFrenzy2)
add_executable(${EXE_NAME} SP_Project.cpp)

# ── Auto-detect compiler and set flags ─────────────────────────────────────
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER})")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message(STATUS "Using GCC flags")
    target_compile_options(${EXE_NAME} PRIVATE -Wall -Wextra -O2)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Using Clang flags")
    target_compile_options(${EXE_NAME} PRIVATE -Wall -Wextra -O2)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(STATUS "Using MSVC flags")
    target_compile_options(${EXE_NAME} PRIVATE /W4 /O2)
endif()

# ── Find SFML dynamically ───────────────────────────────────────────────────
# Prefer find_package; fall back to manual MSYS2/MinGW paths if it fails
set(SFML_COMPONENTS graphics window system audio)

find_package(SFML 3 COMPONENTS ${SFML_COMPONENTS} QUIET)

if(SFML_FOUND)
    message(STATUS "SFML found via find_package")
    if(MSVC)
        # MSVC needs sfml-main on Windows
        target_link_libraries(${EXE_NAME} PRIVATE
            sfml-graphics sfml-window sfml-system sfml-audio sfml-main)
    else()
        target_link_libraries(${EXE_NAME} PRIVATE
            sfml-graphics sfml-window sfml-system sfml-audio)
    endif()

else()
    message(STATUS "find_package failed — trying manual SFML search")

    # Common SFML search paths (covers MSYS2 ucrt64/mingw64, Homebrew, /usr/local)
    set(SFML_SEARCH_PATHS
        "C:/msys64/ucrt64"
        "C:/msys64/mingw64"
        "C:/SFML"
        "/usr/local"
        "/opt/homebrew"          # macOS Apple Silicon
        "/opt/homebrew/opt/sfml"
    )

    find_path(SFML_INCLUDE_DIR
        NAMES SFML/Graphics.hpp
        PATHS ${SFML_SEARCH_PATHS}
        PATH_SUFFIXES include
    )

    find_library(SFML_GRAPHICS_LIB NAMES sfml-graphics PATHS ${SFML_SEARCH_PATHS} PATH_SUFFIXES lib)
    find_library(SFML_WINDOW_LIB   NAMES sfml-window   PATHS ${SFML_SEARCH_PATHS} PATH_SUFFIXES lib)
    find_library(SFML_SYSTEM_LIB   NAMES sfml-system   PATHS ${SFML_SEARCH_PATHS} PATH_SUFFIXES lib)
    find_library(SFML_AUDIO_LIB    NAMES sfml-audio    PATHS ${SFML_SEARCH_PATHS} PATH_SUFFIXES lib)

    if(SFML_INCLUDE_DIR AND SFML_GRAPHICS_LIB)
        message(STATUS "SFML found manually at: ${SFML_INCLUDE_DIR}")
        target_include_directories(${EXE_NAME} PRIVATE ${SFML_INCLUDE_DIR})
        target_link_libraries(${EXE_NAME} PRIVATE
            ${SFML_GRAPHICS_LIB}
            ${SFML_WINDOW_LIB}
            ${SFML_SYSTEM_LIB}
            ${SFML_AUDIO_LIB}
        )
    else()
        message(FATAL_ERROR
            "SFML not found! Install SFML 3 or set CMAKE_PREFIX_PATH to its location.\n"
            "  Example: cmake -DCMAKE_PREFIX_PATH=C:/SFML .."
        )
    endif()
endif()

# ── Platform-specific extras ────────────────────────────────────────────────
if(WIN32)
    # Hide the console window in Release builds on Windows
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set_target_properties(${EXE_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()
endif()

message(STATUS "Build target: ${EXE_NAME}")
message(STATUS "Platform    : ${CMAKE_SYSTEM_NAME}")
message(STATUS "Build type  : ${CMAKE_BUILD_TYPE}")