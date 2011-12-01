# DromeAudioPlayer
set(DromeAudioPlayer_SRCS DromeAudioPlayer.cpp)
add_executable(DromeAudioPlayer ${DromeAudioPlayer_SRCS})

target_link_libraries(
	DromeAudioPlayer
	DromeAudio
)

install(
	TARGETS DromeAudioPlayer
	RUNTIME DESTINATION bin
)
