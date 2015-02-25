IF(ALT_CMAKE)
INCLUDE(altCMakeLists.cmake)
ELSE()

INSTALL( FILES base_dependency_database
               README
         DESTINATION "${product}/${version}/releaseDB" )
ENDIF()
