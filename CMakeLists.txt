cmake_minimum_required(VERSION 3.15)

project(LR2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(LR2 main.cpp)

# Чтобы можно было пересобирать, даже если программа запущена
add_custom_command(TARGET LR2 PRE_LINK
    COMMAND cmd /c "taskkill /IM LR2.exe /F >nul 2>&1 || exit 0"
)