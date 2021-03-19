cmake_minimum_required(VERSION 3.5)
project(phphbase)

option(HBASE_NATIVE_CLIENT_PATH "Path to HBase native client" "/usr/src/hbase-native-client")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(PHP_CONFIG_DIR /etc/php/7.4/cli/conf.d/)
set(PHP_CONFIG php-config)
set(PHPINIFILE phphbase.ini)

set(SOURCE_FILES main.cpp)
add_library(phphbase SHARED main.cpp src/HbaseExtension/Config.cpp include/HbaseExtension/Config.h src/HbaseExtension/Client.cpp include/HbaseExtension/Client.h src/HbaseExtension/Scan.cpp include/HbaseExtension/Scan.h src/HbaseExtension/Scanner.cpp include/HbaseExtension/Scanner.h src/HbaseExtension/ResultConverter.cpp include/HbaseExtension/ResultConverter.h src/HbaseExtension/ParametersChecker.cpp include/HbaseExtension/ParametersChecker.h)

find_library(PHPCPP phpcpp HINTS /usr/lib/)
message(STATUS "Found phpcpp in ${PHPCPP}")
target_link_libraries(phphbase PUBLIC ${PHPCPP})

find_library(GFLAGS gflags HINTS /usr/local/lib/)
message(STATUS "Found gflags in ${GFLAGS}")
target_link_libraries(phphbase PUBLIC ${GFLAGS})

include_directories("${HBASE_NATIVE_CLIENT_PATH}/dependencies/Protobuf-build/include")
include_directories("${HBASE_NATIVE_CLIENT_PATH}/dependencies/boost-install/include")
include_directories("${HBASE_NATIVE_CLIENT_PATH}/dependencies/facebook-folly-proj-install/include")
include_directories("${HBASE_NATIVE_CLIENT_PATH}/dependencies/facebook-wangle-proj-install/include")
include_directories("${HBASE_NATIVE_CLIENT_PATH}/CMakeFiles")
include_directories("${HBASE_NATIVE_CLIENT_PATH}/CMakeFiles/hbase/if")
include_directories("${HBASE_NATIVE_CLIENT_PATH}/include")
include_directories("${HBASE_NATIVE_CLIENT_PATH}/dependencies/zookeeper-install/include")
target_link_libraries(phphbase PUBLIC ${HBASE_NATIVE_CLIENT_PATH}/dependencies/facebook-folly-proj-install/lib/libfolly.so)
target_link_libraries(phphbase PUBLIC ${HBASE_NATIVE_CLIENT_PATH}/libs/libhbaseclient.so)

execute_process(COMMAND ${PHP_CONFIG} --extension-dir OUTPUT_VARIABLE LIBRARY_DIR)

message(STATUS "CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}")
message(STATUS "LIBRARY_DIR ${LIBRARY_DIR}")

add_custom_command(TARGET phphbase
        POST_BUILD
        COMMAND cp -f ${CMAKE_SOURCE_DIR}/${PHPINIFILE}; ${PHP_CONFIG_DIR}
        COMMAND cp -f $<TARGET_FILE:phphbase> ${LIBRARY_DIR}
        )


