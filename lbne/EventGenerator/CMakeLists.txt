IF(ALT_CMAKE)
INCLUDE(altCMakeLists)
ELSE()


# install_headers()
install_fhicl()
install_source()

add_subdirectory(CRY)
add_subdirectory(GENIE)

ENDIF()
