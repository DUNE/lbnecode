# these tests run a FCL file and fail only if lar exits with a bad exit code;
# they are fast and always run.
# FCL files need to be copied to the test area (DATAFILES directive) since they
# are not installed.
cet_test(geometry_lbnefd HANDBUILT
  DATAFILES test_geometry_lbnefd.fcl
  TEST_EXEC lar
  TEST_ARGS --rethrow-all --config ./test_geometry_lbnefd.fcl
)

cet_test(geometry_lbne35t HANDBUILT
  DATAFILES test_geometry_lbne35t.fcl
  TEST_EXEC lar
  TEST_ARGS --rethrow-all --config ./test_geometry_lbne35t.fcl
)

cet_test(geometry_iterator_lbne35t HANDBUILT
  DATAFILES test_geometry_iterators_lbne35t.fcl
  TEST_EXEC lar
  TEST_ARGS --rethrow-all --config ./test_geometry_iterators_lbne35t.fcl
)

cet_test(geometry_iterator_lbnefd HANDBUILT
  DATAFILES test_geometry_iterators_lbnefd.fcl
  TEST_EXEC lar
  TEST_ARGS --rethrow-all --config ./test_geometry_iterators_lbnefd.fcl
)

simple_plugin ( GeometryTest35 "module"
                     Geometry
                     Geometry_service
		     ${ART_FRAMEWORK_CORE}
   		     ${ART_FRAMEWORK_PRINCIPAL}
		     ${ART_FRAMEWORK_SERVICES_REGISTRY}
		     ${ART_FRAMEWORK_SERVICES_BASIC}
		     ${ART_FRAMEWORK_SERVICES_OPTIONAL}
		     ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
             	     ${ART_FRAMEWORK_SERVICES_OPTIONAL_RANDOMNUMBERGENERATOR_SERVICE}
		     ${ART_PERSISTENCY_COMMON}
		     ${ART_PERSISTENCY_PROVENANCE}
		     ${ART_UTILITIES}
                     ${MF_MESSAGELOGGER}
                     ${MF_UTILITIES}
                     ${FHICLCPP}
                     ${CETLIB}
                     ${ROOT_BASIC_LIB_LIST}

               BASENAME_ONLY
          )
