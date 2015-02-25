IF(ALT_CMAKE)
INCLUDE(altCMakeLists)
ELSE()

add_subdirectory(scripts)

# install_headers()
install_fhicl()
install_source()


ENDIF()
