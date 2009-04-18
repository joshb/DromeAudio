cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
	cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

project("DromeAudio")
set(PROJECT_VERSION "0.1.1")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

find_package(ALSA)
if(NOT ALSA_FOUND)
	find_package(OSS)
endif(NOT ALSA_FOUND)
find_package(VorbisFile)

if(WIN32)
	message(STATUS "Using Win32 include directories")

	find_path(
		SDL_INCLUDE_DIR
		SDL/SDL.h
		C:/SDL/include
		C:/GnuWin32/include
	)

	find_library(
		SDL_LIBRARY
		NAMES SDL
		PATHS
		C:/SDL/lib
		C:/GnuWin32/lib
	)

	if(SDL_INCLUDE_DIR AND SDL_LIBRARY)
		set(SDL_FOUND "YES")
	else(SDL_INCLUDE_DIR AND SDL_LIBRARY)
		set(SDL_FOUND "NO")
	endif(SDL_INCLUDE_DIR AND SDL_LIBRARY)
else(WIN32)
	if(NOT ALSA_FOUND AND NOT OSS_FOUND)
		find_package(SDL)
	endif(NOT ALSA_FOUND AND NOT OSS_FOUND)
endif(WIN32)

include_directories(
	include
)

if(ALSA_FOUND)
	include_directories(${ALSA_INCLUDE_DIR})
endif(ALSA_FOUND)

if(OSS_FOUND)
	include_directories(${OSS_INCLUDE_DIR})
endif(OSS_FOUND)

if(SDL_FOUND)
	include_directories(${SDL_INCLUDE_DIR})
endif(SDL_FOUND)

if(VORBISFILE_FOUND)
	include_directories(${VORBISFILE_INCLUDE_DIR})
endif(VORBISFILE_FOUND)

set(CMAKE_CXX_FLAGS -Wall)

if(WIN32)
	set(DROMEAUDIO_LIBRARY_DIR bin)
else(WIN32)
	set(DROMEAUDIO_LIBRARY_DIR lib)
endif(WIN32)

subdirs(include src examples share doc)
