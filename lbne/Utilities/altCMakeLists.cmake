add_subdirectory(test)


set(Utilities_HEADERS
     SignalShapingServiceLBNE10kt.h
     SignalShapingServiceLBNE34kt.h
     SignalShapingServiceLBNE35t.h
     )

art_add_service( SignalShapingServiceLBNE10kt_service SignalShapingServiceLBNE10kt_service.cc )

art_add_service( SignalShapingServiceLBNE34kt_service SignalShapingServiceLBNE34kt_service.cc )

art_add_service( SignalShapingServiceLBNE35t_service SignalShapingServiceLBNE35t_service.cc )

install(TARGETS
     SignalShapingServiceLBNE10kt_service
     SignalShapingServiceLBNE34kt_service
     SignalShapingServiceLBNE35t_service
     EXPORT lbnecodeLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

install(FILES ${Utilities_HEADERS} DESTINATION 
     ${CMAKE_INSTALL_INCLUDEDIR}/lbne/Utilities COMPONENT Development )


file(GLOB FHICL_FILES 
     [^.]*.fcl
     )

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)
