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
