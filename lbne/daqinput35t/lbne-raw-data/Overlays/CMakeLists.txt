IF(ALT_CMAKE)
INCLUDE(altCMakeLists)
ELSE()

art_make(LIB_LIBRARIES
  ${ARTDAQ_DAQDATA}
  ${CETLIB}
  )
install_headers()
install_source()
ENDIF()
