IF(ALT_CMAKE)
INCLUDE(altCMakeLists)
ELSE()
add_subdirectory(single)
add_subdirectory(cosmics)
add_subdirectory(textfile)

install_fhicl()
ENDIF()
