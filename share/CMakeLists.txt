configure_file(FindDromeAudio.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/FindDromeAudio.cmake @ONLY)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/FindDromeAudio.cmake
	DESTINATION ${CMAKE_ROOT}/Modules
)
