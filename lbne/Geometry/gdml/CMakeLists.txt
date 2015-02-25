IF(ALT_CMAKE)
INCLUDE(altCMakeLists.cmake)
ELSE()

install_gdml( SUBDIRS GDMLSchema )

add_subdirectory(lbne)
ENDIF()
