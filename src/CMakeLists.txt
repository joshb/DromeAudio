if(WIN32)
	set(DROMEAUDIO_LIBRARY_DIR bin)
else(WIN32)
	set(DROMEAUDIO_LIBRARY_DIR lib)
endif(WIN32)

subdirs(
	DromeAudio
)
