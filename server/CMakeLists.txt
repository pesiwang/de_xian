CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(de_xian_server.exe)

SET(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -DDEBUG")

INCLUDE_DIRECTORIES(include include/vcs)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)

AUX_SOURCE_DIRECTORY(src SRC_LIST)

ADD_EXECUTABLE(de_xian_server.exe ${SRC_LIST})
TARGET_LINK_LIBRARIES(de_xian_server.exe vcs pthread jsoncpp)



