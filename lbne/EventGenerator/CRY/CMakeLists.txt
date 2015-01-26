IF (CETBUILDTOOLS_VERSION)

# install_headers()
install_fhicl()
install_source()

# ======================================================================
# Everything below this is for an alternate cmake build
# ======================================================================

ELSE()

file(GLOB FHICL_FILES 
     [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)

ENDIF()
