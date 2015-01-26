IF (CETBUILDTOOLS_VERSION)

INSTALL( FILES base_dependency_database
               README
         DESTINATION "${product}/${version}/releaseDB" )

ELSE()

ENDIF()
