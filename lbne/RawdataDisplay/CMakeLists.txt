art_add_module( DbigEVD4apaFD_module DbigEVD4apaFD_module.cc )

art_add_module( RawEVD35t_module RawEVD35t_module.cc )

art_add_module( RawEVD_module RawEVD_module.cc )

file(GLOB FHICL_FILES 
     [^.]*.fcl
     )

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)

install(TARGETS
     DbigEVD4apaFD_module
     RawEVD35t_module
     RawEVD_module
     EXPORT lbnecodeLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

