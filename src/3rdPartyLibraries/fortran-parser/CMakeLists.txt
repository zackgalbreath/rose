
include_directories(${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )

########### install files ###############
set(jar_file ${CMAKE_CURRENT_SOURCE_DIR}/OpenFortranParser-20111001.jar)
install(
  FILES OpenFortranParser-0.8.3.jar
        ${jar_file}
  DESTINATION ${INCLUDE_INSTALL_DIR} )

set(files_to_extract
  "fortran/ofp/parser/c/FortranParserAction.h"
  "fortran/ofp/parser/c/jni/c_actions_jni.c"
  "fortran/ofp/parser/c/token.h"
  "fortran/ofp/parser/c/token.c"
  "fortran/ofp/parser/c/ActionEnums.h"
  "fortran/ofp/parser/c/jni/jni_token_funcs.h"
  "fortran/ofp/parser/c/jni/jni_token_funcs.c"
)

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/ActionEnums.h)
  foreach(file_to_extract ${files_to_extract})
    execute_process(
      COMMAND ${Java_JAR_EXECUTABLE} xf ${jar_file} ${file_to_extract}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    get_filename_component(filename
      ${CMAKE_CURRENT_BINARY_DIR}/${file_to_extract} NAME)
    file(RENAME
      ${CMAKE_CURRENT_BINARY_DIR}/${file_to_extract}
      ${CMAKE_CURRENT_BINARY_DIR}/${filename})
  endforeach()
  file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/fortran)
endif()
