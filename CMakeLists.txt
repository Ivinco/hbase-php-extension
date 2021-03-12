cmake_minimum_required(VERSION 3.5)
project(myext)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


set(PHP_CONFIG_DIR /etc/php/7.4/cli/conf.d/)
#set(PHP_CONFIG_DIR /etc/php.d/)
set(PHP_CONFIG php-config)
set(PHPINIFILE libmyext.ini)

set(SOURCE_FILES main.cpp)
#set(SOURCE_FILES main.cpp src/HbaseExtension/Config.cpp include/HbaseExtension/Config.h)
#add_executable(t ${SOURCE_FILES})

   #add_library(myext SHARED ${SOURCE_FILES})
   add_library(myext SHARED main.cpp src/HbaseExtension/Config.cpp include/HbaseExtension/Config.h src/HbaseExtension/Client.cpp include/HbaseExtension/Client.h)

    find_library(PHPCPP phpcpp HINTS /usr/lib/)
    message(STATUS "Found phpcpp in ${PHPCPP}")
    target_link_libraries(myext PUBLIC ${PHPCPP})

    find_library(GFLAGS gflags HINTS /usr/local/lib/)
    message(STATUS "Found gflags in ${GFLAGS}")
    target_link_libraries(myext PUBLIC ${GFLAGS})

    set(LIB_PATH "/home/julia/Develop/hbase-native-client")
    include_directories("${LIB_PATH}/dependencies/Protobuf-build/include")
    include_directories("${LIB_PATH}/dependencies/boost-install/include")
    include_directories("${LIB_PATH}/dependencies/facebook-folly-proj-install/include")
    include_directories("${LIB_PATH}/dependencies/facebook-wangle-proj-install/include")
    include_directories("${LIB_PATH}/CMakeFiles")
    include_directories("${LIB_PATH}/CMakeFiles/hbase/if")
    include_directories("${LIB_PATH}/include")
    include_directories("${LIB_PATH}/dependencies/zookeeper-install/include")
    target_link_libraries(myext PUBLIC ${LIB_PATH}/dependencies/facebook-folly-proj-install/lib/libfolly.so)
    target_link_libraries(myext PUBLIC ${LIB_PATH}/libs/libhbaseclient.so)

        execute_process(COMMAND ${PHP_CONFIG} --extension-dir OUTPUT_VARIABLE LIBRARY_DIR)

        message(STATUS "CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}")
        message(STATUS "LIBRARY_DIR ${LIBRARY_DIR}")

        add_custom_command(TARGET myext
                POST_BUILD
                COMMAND cp -f ${CMAKE_SOURCE_DIR}/${PHPINIFILE}; ${PHP_CONFIG_DIR}
                COMMAND cp -f $<TARGET_FILE:myext> ${LIBRARY_DIR}
                )


