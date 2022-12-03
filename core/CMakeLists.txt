file(GLOB HEADERS "*.h")
aux_source_directory(. SRC_LIST)

add_library(guodong ${SRC_LIST} ${HEADERS})

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
message(STATUS "output: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
target_include_directories(guodong PUBLIC .)
#PUBLIC: 在public后面的库会被Link到你的target中，并且里面的符号也会被导出，提供给第三方使用
#PRIVATE: 在private后面的库仅被link到你的target中，并且终结掉，第三方不能感知你调了啥库
#INTERFACE: 在interface后面引入的库不会被链接到你的target中，只会导出符号
target_link_libraries(guodong PUBLIC Boost::ASIO Boost::Log Boost::Thread)