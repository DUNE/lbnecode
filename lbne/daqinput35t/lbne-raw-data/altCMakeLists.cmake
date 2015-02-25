IF (CETBUILDTOOLS_VERSION)

# Main library.


# Subdirectories.
add_subdirectory(Overlays)

# ======================================================================
# Everything below this is for an alternate cmake build
# ======================================================================

ELSE()

add_subdirectory(Overlays)

ENDIF()
