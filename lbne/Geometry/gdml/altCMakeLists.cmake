set( gdml_install_dir
     "gdml")

file( GLOB gdml_files
     [^.]*.gdml
     [^.]*.C
     [^.]*.xml
     [^.]*.xsd
     README
     )

set(sub GDMLSchema)

file(GLOB subdir_gdml_files
     ${sub}/[^.]*.gdml
     ${sub}/[^.]*.C
     ${sub}/[^.]*.xml
     ${sub}/[^.]*.xsd
     ${sub}/README
     )

file( GLOB gdml_bin
     [^.]*.pl
     genmake )

install(FILES ${gdml_files} DESTINATION ${gdml_install_dir})
install(FILES ${subdir_gdml_files} DESTINATION ${gdml_install_dir}/GDMLSchema)

add_subdirectory(lbne)
