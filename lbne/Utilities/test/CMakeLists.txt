# cet_enable_asserts()
# 
# art_make( NO_PLUGINS
#           BASENAME_ONLY
#           LIBRARY_NAME  lbne_Utilities
# 	  MODULE_LIBRARIES Utilities
#                         ${ART_FRAMEWORK_CORE}
# 			${ART_FRAMEWORK_PRINCIPAL}
# 			${ART_FRAMEWORK_SERVICES_REGISTRY}
# 			${ART_FRAMEWORK_SERVICES_OPTIONAL}
# 			${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
# 			${ART_PERSISTENCY_COMMON}
# 			${ART_PERSISTENCY_PROVENANCE}
# 			${ART_UTILITIES}
# 			${MF_MESSAGELOGGER}
# 			${MF_UTILITIES}
# 	                ${FHICLCPP}
# 			${CETLIB}
#                         ${ROOT_GEOM}
#                         ${ROOT_XMLIO}
#                         ${ROOT_GDML}
# 			${ROOT_BASIC_LIB_LIST}
#         )
# 
# 
# install_headers()
# install_fhicl()
# install_source()
# 
#

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




