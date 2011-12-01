cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
	cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

message(STATUS "Using gcc warning options")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -Wshadow")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wshadow")

project("DromeAudio")
set(PROJECT_VERSION "0.2.1")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

include_directories(include)

subdirs(include src examples share doc)
