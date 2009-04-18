file(GLOB DromeAudio "${CMAKE_CURRENT_SOURCE_DIR}/DromeAudio/*.h")

install(
	FILES ${DromeAudio}
	DESTINATION include/DromeAudio
)
