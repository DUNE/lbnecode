IF(ALT_CMAKE)
INCLUDE(altCMakeLists)
ELSE()
add_subdirectory(lbne35t)

install_fhicl()
ENDIF()
