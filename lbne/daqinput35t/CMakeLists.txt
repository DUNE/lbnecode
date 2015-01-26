include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

ADD_SUBDIRECTORY(lbne-raw-data)
ADD_SUBDIRECTORY(utilities)
art_add_module( SSPDump_module SSPDump_module.cc )

art_add_module( TpcDAQToOffline_module TpcDAQToOffline_module.cc )

art_add_module( TpcMilliSliceDump_module TpcMilliSliceDump_module.cc )

file(GLOB FHICL_FILES 
     [^.]*.fcl
     )

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)

