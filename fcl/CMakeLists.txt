IF(ALT_CMAKE)
INCLUDE(altCMakeLists.cmake)
ELSE()
add_subdirectory(lbne35t)

install_fhicl()
ENDIF()
