# aux_source_directory(. SRC_LIST)

# file(GLOB HEADERS "*.h")

add_executable(echoserver server.cpp)
add_executable(echoclient client.cpp)
target_link_libraries(echoserver PUBLIC guodong)
target_link_libraries(echoclient PUBLIC guodong)
# install(TARGETS tcpserver DESTINATION bin)