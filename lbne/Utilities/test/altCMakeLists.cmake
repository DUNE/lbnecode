art_add_module(SignalShapingLBNE10ktTest_module SignalShapingLBNE10ktTest_module.cc)

art_add_module(SignalShapingLBNE35tTest_module SignalShapingLBNE35tTest_module.cc)

install(TARGETS
     SignalShapingLBNE10ktTest_module
     SignalShapingLBNE35tTest_module
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

file(GLOB FHICL_FILES 
     [^.]*.fcl
     )

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)
