IF(ALT_CMAKE)
INCLUDE(altCMakeLists.cmake)
ELSE()
# Main library.


# Subdirectories.
add_subdirectory(Overlays)
ENDIF()
