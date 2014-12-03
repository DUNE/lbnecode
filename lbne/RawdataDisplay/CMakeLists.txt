# 
# simple_plugin(RawEVD "module"
# 			Geometry_service
#                         Geometry
# 			RawData
#                         ${SIMULATIONBASE}
#                         ${ART_FRAMEWORK_CORE}
# 			${ART_FRAMEWORK_PRINCIPAL}
# 			${ART_FRAMEWORK_SERVICES_REGISTRY}
#                         ${ART_FRAMEWORK_SERVICES_OPTIONAL}
#                         ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
#  			${ART_PERSISTENCY_COMMON}
# 			${ART_PERSISTENCY_PROVENANCE}
# 			${ART_UTILITIES}
# 			${MF_MESSAGELOGGER}
# 			${MF_UTILITIES}
# 			${CETLIB}
# 			${ROOT_BASIC_LIB_LIST}
#               BASENAME_ONLY
# )
# 
# simple_plugin(RawEVD35t "module"
# 			Geometry_service
#                         Geometry
# 			RawData
#                         ${SIMULATIONBASE}
#                         ${ART_FRAMEWORK_CORE}
# 			${ART_FRAMEWORK_PRINCIPAL}
# 			${ART_FRAMEWORK_SERVICES_REGISTRY}
#                         ${ART_FRAMEWORK_SERVICES_OPTIONAL}
#                         ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
#  			${ART_PERSISTENCY_COMMON}
# 			${ART_PERSISTENCY_PROVENANCE}
# 			${ART_UTILITIES}
# 			${MF_MESSAGELOGGER}
# 			${MF_UTILITIES}
# 			${CETLIB}
# 			${ROOT_BASIC_LIB_LIST}
#               BASENAME_ONLY
# )
# 
# simple_plugin(DbigEVD4apaFD "module"
# 			Geometry_service
#                         Geometry
# 			RawData
# 			RecoBase
#                         ${SIMULATIONBASE}
#                         ${ART_FRAMEWORK_CORE}
# 			${ART_FRAMEWORK_PRINCIPAL}
# 			${ART_FRAMEWORK_SERVICES_REGISTRY}
#                         ${ART_FRAMEWORK_SERVICES_OPTIONAL}
#                         ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
#  			${ART_PERSISTENCY_COMMON}
# 			${ART_PERSISTENCY_PROVENANCE}
# 			${ART_UTILITIES}
# 			${MF_MESSAGELOGGER}
# 			${MF_UTILITIES}
# 			${CETLIB}
# 			${ROOT_BASIC_LIB_LIST}
#               BASENAME_ONLY
# )
# 
# install_headers()
# install_fhicl()
# install_source()
# install_scripts()

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

