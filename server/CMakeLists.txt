CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(de_xian_server.exe)

SET(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -DDEBUG")

INCLUDE_DIRECTORIES(include include/vcs include/protobuf)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)

AUX_SOURCE_DIRECTORY(src SRC_LIST)
AUX_SOURCE_DIRECTORY(src/protobuf SRC_LIST)
AUX_SOURCE_DIRECTORY(src/model SRC_LIST)

ADD_EXECUTABLE(de_xian_server.exe ${SRC_LIST})
TARGET_LINK_LIBRARIES(de_xian_server.exe vcs protobuf pthread jsoncpp)

AUX_SOURCE_DIRECTORY(test TEST_SRC_LIST)
AUX_SOURCE_DIRECTORY(src/protobuf TEST_SRC_LIST)
ADD_EXECUTABLE(de_xian_test.exe ${TEST_SRC_LIST})
TARGET_LINK_LIBRARIES(de_xian_test.exe vcs protobuf)


