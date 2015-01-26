IF (CETBUILDTOOLS_VERSION)

art_make( BASENAME_ONLY
          MODULE_LIBRARIES Geometry
			Geometry_service
			Simulation
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
	                ${FHICLCPP}
			${CETLIB}
                        ${ROOT_GEOM}
                        ${ROOT_XMLIO}
                        ${ROOT_GDML}
			${ROOT_BASIC_LIB_LIST}
        )

# install_headers()
install_fhicl()
install_source()

# ======================================================================
# Everything below this is for an alternate cmake build
# ======================================================================

ELSE()

file(GLOB FHICL_FILES 
     [^.]*.fcl
     )

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)

ENDIF()
