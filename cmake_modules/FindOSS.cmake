if(WIN32)
	set(OSS_FOUND "NO")
else(WIN32)
	find_path(
		OSS_INCLUDE_DIR
		soundcard.h
		/usr/include/linux
		/usr/include/sys
	)

	if(OSS_INCLUDE_DIR)
		set(OSS_FOUND "YES")
	else(OSS_INCLUDE_DIR)
		set(OSS_FOUND "NO")
	endif(OSS_INCLUDE_DIR)
endif(WIN32)

if(OSS_FOUND)
	message(STATUS "Found OSS header")
else(OSS_FOUND)
	message(STATUS "Could not find OSS")
endif(OSS_FOUND)
