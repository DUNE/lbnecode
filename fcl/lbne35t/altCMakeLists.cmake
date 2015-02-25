add_subdirectory(gen)
add_subdirectory(g4)
add_subdirectory(detsim)
add_subdirectory(reco)
add_subdirectory(mergeana)

file(GLOB FHICL_FILES
 [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)
