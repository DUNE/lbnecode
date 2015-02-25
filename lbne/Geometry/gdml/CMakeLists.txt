IF(ALT_CMAKE)
INCLUDE(altCMakeLists)
ELSE()

install_gdml( SUBDIRS GDMLSchema )

add_subdirectory(lbne)
ENDIF()
