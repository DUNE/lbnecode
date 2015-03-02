art_add_module( SimWireLBNE10kt_module SimWireLBNE10kt_module.cc )

art_add_module( SimWireLBNE35t_module SimWireLBNE35t_module.cc )

install(TARGETS
     SimWireLBNE10kt_module
     SimWireLBNE35t_module
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
