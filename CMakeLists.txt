cmake_minimum_required(VERSION 3.12)
project(guodong VERSION 1.0.0 LANGUAGES CXX)

# 添加cmake模块目录（CMAKE_CURRENT_SOURCE_DIR表示当前CMakeList.txt的路径）
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
        "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()
set(CMAKE_CXX_FLAGS "-pthread -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -fexceptions")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/build/")

# 基本设置
include(ProjectBoost)
include(ProjectTASSL)

add_subdirectory(core)
add_subdirectory(example)