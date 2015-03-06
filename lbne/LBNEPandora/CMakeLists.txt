
# where should the scripts/..xml file be installed?  Perhaps in bin?

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

include_directories( $ENV{PANDORA_INC} )
include_directories( $ENV{PANDORA_FQ_DIR}/PandoraSDK/include )
include_directories( $ENV{PANDORA_FQ_DIR}/PandoraMonitoring/include )
include_directories( $ENV{LARPANDORA_INC}/LArPandoraAlgorithms )
include_directories( $ENV{LARPANDORA_INC}/LArPandoraInterface )

art_make( BASENAME_ONLY
          LIBRARY_NAME  LBNEPandora
          LIB_LIBRARIES LArPandoraInterface
                        LArPandoraAlgorithms
                        Geometry
                        Geometry_service
                        Simulation
                        ${PANDORASDK}
                        ${PANDORAMONITORING}
                        ${SIMULATIONBASE}
                        ${ART_FRAMEWORK_CORE}
                        ${ART_FRAMEWORK_PRINCIPAL}
                        ${ART_FRAMEWORK_SERVICES_REGISTRY}
                        ${ART_FRAMEWORK_SERVICES_BASIC}
                        ${ART_FRAMEWORK_SERVICES_OPTIONAL}
                        ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
                        ${ART_PERSISTENCY_COMMON}
                        ${ART_PERSISTENCY_PROVENANCE}
                        ${ART_UTILITIES}
                        ${MF_MESSAGELOGGER}
                        ${MF_UTILITIES}
                        ${FHICLCPP}
                        ${CETLIB}
                        ${ROOT_BASIC_LIB_LIST}
          MODULE_LIBRARIES LArPandoraInterface 
                           LBNEPandora
          )

install_headers()
install_fhicl()
install_source()

add_subdirectory(scripts)