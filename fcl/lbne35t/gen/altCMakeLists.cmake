IF (CETBUILDTOOLS_VERSION)

add_subdirectory(single)
add_subdirectory(cosmics)
add_subdirectory(textfile)

install_fhicl()

# ======================================================================
# Everything below this is for an alternate cmake build
# ======================================================================

ELSE()

file(GLOB FHICL_FILES
 [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)


ENDIF()
