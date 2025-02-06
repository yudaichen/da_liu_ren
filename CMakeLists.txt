cmake_minimum_required(VERSION 3.28)
project(da_liu_ren)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/LunarCalendar-master/src)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# 定义源文件
set(SOURCES
        main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/liu_ren.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/liu_ren.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/common.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/LunarCalendar-master/src/lunar.h
        ${CMAKE_CURRENT_SOURCE_DIR}/LunarCalendar-master/src/lunar.cpp
)

add_subdirectory(third_party/fmt)

# 创建可执行文件
add_executable(da_liu_ren ${SOURCES})

target_link_libraries(da_liu_ren PRIVATE fmt::fmt)