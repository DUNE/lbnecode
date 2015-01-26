IF (CETBUILDTOOLS_VERSION)
# Install fcl files in /job subdirectory.

install_fhicl()

# Also put a copy in the source tree.

FILE(GLOB fcl_files *.fcl)
install_source( EXTRAS ${fcl_files} )

# ======================================================================
# Everything below this is for an alternate cmake build
# ======================================================================

ELSE()

file(GLOB FHICL_FILES
 [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)


ENDIF()
