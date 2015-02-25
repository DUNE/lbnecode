set(Overlays_HEADERS
     FragmentType.hh
     MicroSlice.hh
     MicroSliceWriter.hh
     MilliSlice.hh
     MilliSliceFragment.hh
     MilliSliceFragmentWriter.hh
     MilliSliceWriter.hh
     NanoSlice.hh
     NanoSliceWriter.hh
     SSPFragment.hh
     SSPFragmentWriter.hh
     ToyFragment.hh
     ToyFragmentWriter.hh
     TpcMicroSlice.hh
     TpcMilliSlice.hh
     TpcMilliSliceFragment.hh
     TpcMilliSliceWriter.hh
     TpcNanoSlice.hh
     anlTypes.hh
     )

set(Overlays_SOURCES
     FragmentType.cc
     MicroSlice.cc
     MicroSliceWriter.cc
     MilliSlice.cc
     MilliSliceFragment.cc
     MilliSliceFragmentWriter.cc
     MilliSliceWriter.cc
     NanoSlice.cc
     NanoSliceWriter.cc
     SSPFragment.cc
     ToyFragment.cc
     TpcMicroSlice.cc
     TpcMilliSlice.cc
     TpcMilliSliceFragment.cc
     TpcMilliSliceWriter.cc
     TpcNanoSlice.cc
     )

add_library(lbne_daqinput35t_lbne-raw-data_Overlays SHARED
     ${Overlays_HEADERS}
     ${Overlays_SOURCES}
     )

target_link_libraries(lbne_daqinput35t_lbne-raw-data_Overlays
     ${ARTDAQ_DAQDATA}
     FNALCore::FNALCore
     )

install(TARGETS
     lbne_daqinput35t_lbne-raw-data_Overlays 
     EXPORT lbnecodeLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime 
     )

install(FILES ${Overlays_HEADERS} DESTINATION 
     ${CMAKE_INSTALL_INCLUDEDIR}/lbne/daqinput35t/lbne-raw-data/Overlays COMPONENT Development )
