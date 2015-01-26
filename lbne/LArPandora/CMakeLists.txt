IF (CETBUILDTOOLS_VERSION)

add_subdirectory(scripts)

# install_headers()
install_fhicl()
install_source()

ELSE()

file(GLOB FHICL_FILES
 [^.]*.fcl
 )

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)

ENDIF()
