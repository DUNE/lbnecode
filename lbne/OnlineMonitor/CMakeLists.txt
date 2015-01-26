IF (CETBUILDTOOLS_VERSION)

simple_plugin(RawEVD35tTree "module"
			Geometry_service
                        Geometry
			RawData
                        ${SIMULATIONBASE}
                        ${ART_FRAMEWORK_CORE}
			${ART_FRAMEWORK_PRINCIPAL}
			${ART_FRAMEWORK_SERVICES_REGISTRY}
                        ${ART_FRAMEWORK_SERVICES_OPTIONAL}
                        ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
 			${ART_PERSISTENCY_COMMON}
			${ART_PERSISTENCY_PROVENANCE}
			${ART_UTILITIES}
			${MF_MESSAGELOGGER}
			${MF_UTILITIES}
			${CETLIB}
			${ROOT_BASIC_LIB_LIST}
              BASENAME_ONLY
)


install_headers()
install_fhicl()
install_source()
install_scripts()

# ======================================================================
# Everything below this is for an alternate cmake build
# ======================================================================

ELSE()

art_add_module( RawEVD35tTree_module RawEVD35tTree_module.cc )

install(TARGETS
     RawEVD35tTree_module
     EXPORT lbnecodeLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

file(GLOB FHICL_FILES 
     [^.]*.fcl
     )

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)

install(FILES ${OnlineMonitor_HEADERS} DESTINATION 
     ${CMAKE_INSTALL_INCLUDEDIR}/lbne/OnlineMonitor COMPONENT Development )


ENDIF()
