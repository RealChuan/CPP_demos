#设定版本号
cmake_minimum_required(VERSION 3.5)

#设定工程名
project(examples LANGUAGES C CXX)

include_directories( ${PROJECT_SOURCE_DIR} )

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

#设置编译选项
set(CXX_FLAGS
    -g
    # -DVALGRIND
    -DCHECK_PTHREAD_RETURN_VALUE
    -D_FILE_OFFSET_BITS=64
    -Wall
    -Wextra
    -Werror
    # -Wconversion
    -Wno-unused-parameter
    # -Wold-style-cast
    -Woverloaded-virtual
    -Wpointer-arith
    -Wshadow
    -Wwrite-strings
    -march=native
    -MMD
    -std=c++11
    -rdynamic
    )

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

#设定可执行二进制文件的目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)  #源文件目录
#SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)     #输出文件目录

#设定存放编译出来的库文件的目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

#并且把该目录设为连接目录
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
#LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

include_directories( ${PROJECT_SOURCE_DIR} )

if (CMAKE_HOST_WIN32)
    set(WINDOWS 1)
elseif (CMAKE_HOST_APPLE)
    set(MACOS 1)
elseif (CMAKE_HOST_UNIX)
    set(LINUX 1)
endif ()

#增加子文件夹(注意我们的src路径是通过子文件夹的形式添加的，在这里面也是可以写CMakeLists.txt的)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/Algorithms)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/Crc)
ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/List)

IF (WINDOWS)
	MESSAGE(STATUS "Now is windows。")
    ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/CrashHandler)
    ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/WatchDirectory)
ELSEIF (LINUX)
	MESSAGE(STATUS "Now is LINUX.")
    ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/Client)
    ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/Icmp)
    ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/Process)
    ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/Server)
ENDIF ()
