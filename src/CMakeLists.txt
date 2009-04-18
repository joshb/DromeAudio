set(
	SRCS
	AudioDriver.cpp
	Endian.cpp
	Mixer.cpp
	Mutex.cpp
	NoiseSound.cpp
	Sample.cpp
	SawSound.cpp
	SineSound.cpp
	Sound.cpp
	SoundEffect.cpp
	SquareSound.cpp
	TriangleSound.cpp
	Util.cpp
	WavSound.cpp
)

if(ALSA_FOUND)
	add_definitions(-DWITH_ALSA)
	set(SRCS ${SRCS} AudioDriverALSA.cpp)
endif(ALSA_FOUND)

if(OSS_FOUND)
	add_definitions(-DWITH_OSS)
	set(SRCS ${SRCS} AudioDriverOSS.cpp)
endif(OSS_FOUND)

if(SDL_FOUND)
	add_definitions(-DWITH_SDL)
	set(SRCS ${SRCS} AudioDriverSDL.cpp)
endif(SDL_FOUND)

if(VORBISFILE_FOUND)
	add_definitions(-DWITH_VORBIS)
	set(SRCS ${SRCS} VorbisSound.cpp)
endif(VORBISFILE_FOUND)

add_library(DromeAudio SHARED ${SRCS})

if(ALSA_FOUND)
	target_link_libraries(DromeAudio ${ALSA_LIBRARY})
endif(ALSA_FOUND)

if(SDL_FOUND)
	target_link_libraries(DromeAudio ${SDL_LIBRARY})
endif(SDL_FOUND)

if(VORBISFILE_FOUND)
	target_link_libraries(DromeAudio ${VORBISFILE_LIBRARY})
endif(VORBISFILE_FOUND)

install(
	TARGETS DromeAudio
	RUNTIME DESTINATION ${DROMEAUDIO_LIBRARY_DIR}
	LIBRARY DESTINATION ${DROMEAUDIO_LIBRARY_DIR}
)
