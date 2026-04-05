cmake_minimum_required(VERSION 3.16)
project(SP_Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(TEST SP_Project.cpp)

# SFML handling per platform
if(WIN32)
    # Windows
    if(MINGW)
        # MSYS2 / MinGW
        include_directories("C:/msys64/ucrt64/include")
        target_link_libraries(TEST PRIVATE
            sfml-graphics
            sfml-window
            sfml-system
            sfml-audio
        )
    elseif(MSVC)
        # Visual Studio (assumes SFML installed somewhere)
        find_package(SFML 3 COMPONENTS graphics window system audio REQUIRED)
        target_link_libraries(TEST PRIVATE sfml-graphics sfml-window sfml-system sfml-audio sfml-main)
    endif()
elseif(UNIX)
    # Linux / macOS
    find_package(SFML 3 COMPONENTS graphics window system audio REQUIRED)
    target_link_libraries(TEST PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
endif()