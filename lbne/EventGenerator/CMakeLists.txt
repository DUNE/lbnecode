IF(ALT_CMAKE)
INCLUDE(altCMakeLists.cmake)
ELSE()


# install_headers()
install_fhicl()
install_source()

add_subdirectory(CRY)
add_subdirectory(GENIE)

ENDIF()
