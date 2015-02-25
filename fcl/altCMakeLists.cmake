IF (CETBUILDTOOLS_VERSION)

add_subdirectory(lbne35t)

install_fhicl()

# ======================================================================
# Everything below this is for an alternate cmake build
# ======================================================================

ELSE()

add_subdirectory(lbne35t)

file(GLOB FHICL_FILES
 [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)

ENDIF()
