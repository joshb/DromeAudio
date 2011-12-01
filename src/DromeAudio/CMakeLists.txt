set(
	SRCS
	AudioContext.cpp
	AudioDriver.cpp
	Endian.cpp
	Mutex.cpp
	Sample.cpp
	SineSound.cpp
	Sound.cpp
	SoundEffect.cpp
	SoundEmitter.cpp
	Util.cpp
	WavSound.cpp
)

if(APPLE)
	# link to Cocoa
	find_library(COCOA_LIBRARY Cocoa)
	set(LIBS ${LIBS} ${COCOA_LIBRARY})

	# include path to Cocoa headers
	find_path(COCOA_INCLUDE_DIR Cocoa/Cocoa.h)
	include_directories(${COCOA_INCLUDE_DIR})

	# link to CoreAudio
	find_library(COREAUDIO_LIBRARY CoreAudio)
	set(LIBS ${LIBS} ${COREAUDIO_LIBRARY})

	# include path to CoreServices headers
	find_path(CORESERVICES_INCLUDE_DIR CoreServices/CoreServices.h)
	include_directories(${CORESERVICES_INCLUDE_DIR})

	# link to AudioUnit
	find_library(AUDIOUNIT_LIBRARY AudioUnit)
	set(LIBS ${LIBS} ${AUDIOUNIT_LIBRARY})

	# include path to AudioUnit headers
	find_path(AUDIOUNIT_INCLUDE_DIR AudioUnit/AudioUnit.h)
	include_directories(${AUDIOUNIT_INCLUDE_DIR})

	# link to AudioToolbox
	find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox)
	set(LIBS ${LIBS} ${AUDIOTOOLBOX_LIBRARY})

	# build with OS X support
	set(SRCS ${SRCS} AudioDriverOSX.cpp CoreAudioSound.cpp)
	add_definitions(-DWITH_OSX)
else(APPLE)
	find_package(ALSA)
	if(ALSA_FOUND)
		# link to ALSA and include path to ALSA headers
		set(LIBS ${LIBS} ${ALSA_LIBRARY})
		include_directories(${ALSA_INCLUDE_DIR})

		# build with ALSA support
		set(SRCS ${SRCS} AudioDriverALSA.cpp)
		add_definitions(-DWITH_ALSA)
	else(ALSA_FOUND)
		if(SDL_FOUND)
			# link to SDL and include path to SDL headers
			set(LIBS ${LIBS} ${SDL_LIBRARY})
			include_directories(${SDL_INCLUDE_DIR})

			# build with SDL support
			set(SRCS ${SRCS} AudioDriverSDL.cpp)
			add_definitions(-DWITH_SDL)
		endif(SDL_FOUND)
	endif(ALSA_FOUND)
endif(APPLE)

find_package(VorbisFile)
if(VORBISFILE_FOUND)
	# link to VorbisFile and include path to VorbisFile headers
	set(LIBS ${LIBS} ${VORBISFILE_LIBRARY})
	include_directories(${VORBISFILE_INCLUDE_DIR})

	# build with Vorbis support
	set(SRCS ${SRCS} VorbisSound.cpp)
	add_definitions(-DWITH_VORBIS)
endif(VORBISFILE_FOUND)

add_library(DromeAudio STATIC ${SRCS})
target_link_libraries(
	DromeAudio
	${LIBS}
)

install(
	TARGETS DromeAudio
	RUNTIME DESTINATION ${DROMEAUDIO_LIBRARY_DIR}
	LIBRARY DESTINATION ${DROMEAUDIO_LIBRARY_DIR}
	ARCHIVE DESTINATION ${DROMEAUDIO_LIBRARY_DIR}
)
