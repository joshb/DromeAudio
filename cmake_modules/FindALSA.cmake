if(WIN32)
	set(ALSA_FOUND "NO")
else(WIN32)
	find_path(
		ALSA_INCLUDE_DIR
		alsa/asoundlib.h
		/usr/include
		/usr/local/include
	)

	find_library(
		ALSA_LIBRARY
		NAMES asound
		PATHS
		/usr/lib
		/usr/local/lib
	)

	if(ALSA_INCLUDE_DIR AND ALSA_LIBRARY)
		set(ALSA_FOUND "YES")
	else(ALSA_INCLUDE_DIR AND ALSA_LIBRARY)
		set(ALSA_FOUND "NO")
	endif(ALSA_INCLUDE_DIR AND ALSA_LIBRARY)
endif(WIN32)

if(ALSA_FOUND)
	message(STATUS "Found ALSA: ${ALSA_LIBRARY}")
else(ALSA_FOUND)
	message(STATUS "Could not find ALSA")
endif(ALSA_FOUND)
