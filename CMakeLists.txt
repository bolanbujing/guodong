cmake_minimum_required(VERSION 3.12)
project(guodong VERSION 1.0.0 LANGUAGES CXX)

# 添加模块目录（CMAKE_CURRENT_SOURCE_DIR表示当前CMakeList.txt的路径）
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTATICLIB")

# 基本设置
include(Options)
include(CompilerSettings)
#print_config()

# 下载安装依赖库

include(ProjectBoost)
include(ProjectTASSL)

aux_source_directory(. SRC_LIST)

file(GLOB HEADERS "*.h")

add_library(guodong ${SRC_LIST} ${HEADERS})


target_include_directories(guodong PRIVATE ..)
#PUBLIC: 在public后面的库会被Link到你的target中，并且里面的符号也会被导出，提供给第三方使用
#PRIVATE: 在private后面的库仅被link到你的target中，并且终结掉，第三方不能感知你调了啥库
#INTERFACE: 在interface后面引入的库不会被链接到你的target中，只会导出符号
target_link_libraries(guodong PUBLIC Boost::ASIO Boost::Log Boost::Thread)