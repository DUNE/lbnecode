IF(ALT_CMAKE)
INCLUDE(altCMakeLists)
ELSE()

INSTALL( FILES base_dependency_database
               README
         DESTINATION "${product}/${version}/releaseDB" )
ENDIF()
