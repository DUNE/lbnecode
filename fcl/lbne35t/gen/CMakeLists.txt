IF(ALT_CMAKE)
INCLUDE(altCMakeLists.cmake)
ELSE()
add_subdirectory(single)
add_subdirectory(cosmics)
add_subdirectory(textfile)

install_fhicl()
ENDIF()
