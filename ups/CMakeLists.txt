IF(ALT_CMAKE)
INCLUDE(altCMakeLists)
ELSE()

# create package configuration and version files

process_ups_files()

cet_cmake_config()
ENDIF()
