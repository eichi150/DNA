cmake_minimum_required(VERSION 3.10)

project(DNA)

# C++17 Standard setzen
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Quellen
set(SOURCES
    main.cpp
    airplane.cpp
    animator.cpp
    circle.cpp
)

# SFML suchen
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Executable erstellen
add_executable(${PROJECT_NAME} ${SOURCES})

# SFML-Bibliotheken linken
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)