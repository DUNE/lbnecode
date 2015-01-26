IF (CETBUILDTOOLS_VERSION)

add_subdirectory(gen)
add_subdirectory(g4)
add_subdirectory(detsim)
add_subdirectory(reco)
add_subdirectory(mergeana)

install_fhicl()

# ======================================================================
# Everything below this is for an alternate cmake build
# ======================================================================


ELSE()

add_subdirectory(gen)
add_subdirectory(g4)
add_subdirectory(detsim)
add_subdirectory(reco)
add_subdirectory(mergeana)

file(GLOB FHICL_FILES
 [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)


ENDIF()
