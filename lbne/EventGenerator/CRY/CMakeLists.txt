IF(ALT_CMAKE)
INCLUDE(altCMakeLists.cmake)
ELSE()


# install_headers()
install_fhicl()
install_source()


ENDIF()
