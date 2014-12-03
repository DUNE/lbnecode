# art_make(LIB_LIBRARIES
#   ${ARTDAQ_DAQDATA}
#   ${CETLIB}
#   )
# install_headers()
# install_source()

set(utilities_HEADERS
     UnpackFragment.h
     )

set(utilities_SOURCES
     UnpackFragment.cxx
     )


add_library(lbne_daqinput35t_utilities SHARED
     ${utilities_HEADERS}
     ${utilities_SOURCES}
     )

target_link_libraries(lbne_daqinput35t_utilities 
     ${ARTDAQ_DAQDATA}
     FNALCore::FNALCore
     )

install(TARGETS
     lbne_daqinput35t_utilities
     EXPORT lbnecodeLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

install(FILES ${utilities_HEADERS} DESTINATION 
     ${CMAKE_INSTALL_INCLUDEDIR}/lbne/daqinput35t/utilities COMPONENT Development )

