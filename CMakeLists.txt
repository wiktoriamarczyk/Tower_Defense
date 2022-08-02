#*****************************************************
# specify minimum version of CMake to use
# (find_package requires 3.23)
#*****************************************************
cmake_minimum_required(VERSION 3.23)

#*****************************************************
project(TowerDefense)
#*****************************************************

#*****************************************************
# add source files
#*****************************************************

SET(APP_SRC_FILES_LIST
    TowerDefense/Button.cpp
    TowerDefense/Button.h
    TowerDefense/Common.cpp
    TowerDefense/Common.h
    TowerDefense/Engine.cpp
    TowerDefense/Engine.h
    TowerDefense/Font.cpp
    TowerDefense/Font.h
    TowerDefense/GameObject.cpp
    TowerDefense/GameObject.h
    TowerDefense/GameState.cpp
    TowerDefense/GameState.h
    TowerDefense/InGameState.cpp
    TowerDefense/InGameState.h
    TowerDefense/MainMenuState.cpp
    TowerDefense/MainMenuState.h
    TowerDefense/Sound.cpp
    TowerDefense/Sound.h
    TowerDefense/Source.cpp
    TowerDefense/Texture.cpp
    TowerDefense/Texture.h
    TowerDefense/Tower.cpp
    TowerDefense/Tower.h
    TowerDefense/vec2.cpp
    TowerDefense/vec2.h
)

#*****************************************************
# Find SFML library (set proper path and run find_package)
#*****************************************************

SET(SFML_DIR "./Dependencies/SFML-2.5.1/lib/cmake/SFML" )
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

#*****************************************************
# generate app exe
#*****************************************************

ADD_EXECUTABLE(TowerDefense ${APP_SRC_FILES_LIST} )

#*****************************************************
# enable multi processor compilation 
#  - for visual "/MP" flag must be set
#*****************************************************

if(MSVC)
    target_compile_options(TowerDefense PRIVATE "/MP")
endif()

#*****************************************************
# Set language version to C++20
#*****************************************************

set_property(TARGET TowerDefense PROPERTY CXX_STANDARD 20)

#*****************************************************
# Specify which SFML libraries to link with exe
#*****************************************************

target_link_libraries(TowerDefense sfml-graphics sfml-audio)