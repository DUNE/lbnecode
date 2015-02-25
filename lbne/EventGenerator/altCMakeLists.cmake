file(GLOB FHICL_FILES 
     [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)

add_subdirectory(CRY)
add_subdirectory(GENIE)
