cmake_minimum_required(VERSION 3.21)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# csgo.dll
project(csgo)
add_library(csgo SHARED
    src/CSGO/DllMain.cpp
)

# injector.exe
project(injector)
add_executable(injector src/injector.cpp)
