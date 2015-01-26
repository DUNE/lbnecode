
add_subdirectory(gdml)

set(Geometry_HEADERS
     ChannelMap35Alg.h
     ChannelMapAPAAlg.h
     GeoObjectSorter35.h
     GeoObjectSorterAPA.h
     LBNEGeometryHelper.h
     )

set(Geometry_SOURCES
     ChannelMap35Alg.cxx
     ChannelMapAPAAlg.cxx
     GeoObjectSorter35.cxx
     GeoObjectSorterAPA.cxx
     )

add_library(lbne_Geometry SHARED
     ${Geometry_HEADERS}
     ${Geometry_SOURCES}
     )

target_link_libraries(lbne_Geometry
     art::art_Framework_Core
     art::art_Framework_Principal
     art::art_Persistency_Provenance
     art::art_Utilities
     art::art_Framework_Services_Registry
     FNALCore::FNALCore
     )

art_add_service( LBNEGeometryHelper_service LBNEGeometryHelper_service.cc )

install(TARGETS
     lbne_Geometry
     LBNEGeometryHelper_service
     EXPORT lbnecodeLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

install(FILES ${Geometry_HEADERS} DESTINATION 
     ${CMAKE_INSTALL_INCLUDEDIR}/lbne/Geometry COMPONENT Development )

file(GLOB FHICL_FILES 
     [^.]*.fcl
     )

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)

