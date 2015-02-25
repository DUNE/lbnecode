IF(ALT_CMAKE)
INCLUDE(altCMakeLists)
ELSE()
# Main library.


# Subdirectories.
add_subdirectory(Overlays)
ENDIF()
