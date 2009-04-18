if(WIN32)
	find_path(
		VORBISFILE_INCLUDE_DIR
		vorbisfile.h
		C:/ogg/include
	)

	find_library(
		VORBISFILE_LIBRARY
		NAMES vorbisfile
		PATHS
		C:/ogg/lib
	)
else(WIN32)
	find_path(
		VORBISFILE_INCLUDE_DIR
		vorbisfile.h
		/usr/include/vorbis
		/usr/local/include/vorbis
	)

	find_library(
		VORBISFILE_LIBRARY
		NAMES vorbisfile
		PATHS
		/usr/lib
		/usr/local/lib
	)
endif(WIN32)

if(VORBISFILE_INCLUDE_DIR AND VORBISFILE_LIBRARY)
	set(VORBISFILE_FOUND "YES")
else(VORBISFILE_INCLUDE_DIR AND VORBISFILE_LIBRARY)
	set(VORBISFILE_FOUND "NO")
endif(VORBISFILE_INCLUDE_DIR AND VORBISFILE_LIBRARY)

if(VORBISFILE_FOUND)
	message(STATUS "Found vorbisfile: ${VORBISFILE_LIBRARY}")
else(VORBISFILE_FOUND)
	message(STATUS "Could not find vorbisfile")
endif(VORBISFILE_FOUND)
