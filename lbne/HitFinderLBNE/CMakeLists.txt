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


