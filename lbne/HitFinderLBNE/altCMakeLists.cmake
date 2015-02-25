IF (CETBUILDTOOLS_VERSION)

art_make( BASENAME_ONLY
          LIBRARY_NAME     HitFinderLBNE
	  LIB_LIBRARIES    RecoBase
	  		   RecoAlg
			   Filters
                           Geometry
                           Geometry_service
                           RecoObjects
                           LArProperties_service
                           DetectorProperties_service
                           Utilities
                           ${ART_FRAMEWORK_CORE}
   			   ${ART_FRAMEWORK_PRINCIPAL}
			   ${ART_FRAMEWORK_SERVICES_REGISTRY}
			   ${ART_FRAMEWORK_SERVICES_BASIC}
			   ${ART_FRAMEWORK_SERVICES_OPTIONAL}
			   ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
                           ${ART_FRAMEWORK_SERVICES_OPTIONAL_RANDOMNUMBERGENERATOR_SERVICE}
			   ${ART_PERSISTENCY_COMMON}
		       	   ${ART_PERSISTENCY_PROVENANCE}
			   ${ART_UTILITIES}
			   ${MF_MESSAGELOGGER}
			   ${MF_UTILITIES}
	                   ${FHICLCPP}
			   ${CETLIB}
                           ${CLHEP}
			   ${ROOT_BASIC_LIB_LIST}
         MODULE_LIBRARIES HitFinderLBNE
	  		   RecoBase
                           Geometry
                           Geometry_service
                           LArProperties_service
                           DetectorProperties_service
                           Utilities
 			   ${ART_FRAMEWORK_CORE}
			   ${ART_FRAMEWORK_PRINCIPAL}
			   ${ART_FRAMEWORK_SERVICES_REGISTRY}
			   ${ART_FRAMEWORK_SERVICES_BASIC}
			   ${ART_FRAMEWORK_SERVICES_OPTIONAL}
			   ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
			   ${ART_PERSISTENCY_COMMON}
			   ${ART_UTILITIES}
			   ${MF_MESSAGELOGGER}
			   ${MF_UTILITIES}
	                   ${FHICLCPP}
			   ${CETLIB}
                           ${ROOT_BASIC_LIB_LIST}
         )

install_headers()
install_fhicl()
install_source()

# ======================================================================
# Everything below this is for an alternate cmake build
# ======================================================================

ELSE()

set(HitFinderLBNE_HEADERS
     APAGeometryAlg.h
     DisambigAlg35t.h
     )

set(HitFinderLBNE_SOURCES
     APAGeometryAlg.cxx
     DisambigAlg35t.cxx
     ) 

add_library(HitFinderLBNE SHARED
     ${HitFinderLBNE_HEADERS}
     ${HitFinderLBNE_SOURCES}
     )

target_link_libraries(HitFinderLBNE
     art::art_Framework_Core
     art::art_Framework_Principal
     art::art_Persistency_Provenance
     art::art_Utilities
     art::art_Framework_Services_Registry
     FNALCore::FNALCore
     )

art_add_module(HitFinder35t_module HitFinder35t_module.cc)

install(TARGETS
     HitFinderLBNE
     HitFinder35t_module
     EXPORT lbnecodeLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

install(FILES ${HitFinderLBNE_HEADERS} DESTINATION 
     ${CMAKE_INSTALL_INCLUDEDIR}/lbne/HitFinderLBNE COMPONENT Development )

file(GLOB FHICL_FILES 
     [^.]*.fcl
     )

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)

ENDIF()
