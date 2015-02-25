IF (CETBUILDTOOLS_VERSION)

art_make( BASENAME_ONLY
          MODULE_LIBRARIES Geometry
	                lbne_Geometry
			Geometry_service
	                Utilities
			Filters
			RawData
			RecoBase
                        SignalShapingServiceLBNE10kt_service
			SignalShapingServiceLBNE35t_service
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

art_add_module( CalGausHFLBNE10kt_module CalGausHFLBNE10kt_module.cc )

art_add_module( CalGausHFLBNE35t_module CalGausHFLBNE35t_module.cc )

art_add_module( CalWireLBNE10kt_module CalWireLBNE10kt_module.cc ) 

art_add_module( CalWireLBNE35t_module CalWireLBNE35t_module.cc )

art_add_module( DeconvGausHFLBNE10kt_module DeconvGausHFLBNE10kt_module.cc )

art_add_module( DeconvGausHFLBNE35t_module DeconvGausHFLBNE35t_module.cc )

install(TARGETS
     CalGausHFLBNE10kt_module
     CalGausHFLBNE35t_module
     CalWireLBNE10kt_module
     CalWireLBNE35t_module
     DeconvGausHFLBNE10kt_module
     DeconvGausHFLBNE35t_module
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


ENDIF()
