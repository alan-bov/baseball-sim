cmake_minimum_required(VERSION 3.15)

project(BBModelSim LANGUAGES C CXX)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(sim
    main.cpp
    scripts/sqlite3.c
    scripts/get_data.cpp
    scripts/pitch_select.cpp
)

target_include_directories(sim PRIVATE ${CMAKE_SOURCE_DIR}/scripts)

# Add pthread and dl (if on Linux/macOS)
if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(sim PRIVATE Threads::Threads dl)
endif()