# 
# 
# # install_headers()
# install_fhicl()
# install_source()
# 
file(GLOB FHICL_FILES 
     [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)



