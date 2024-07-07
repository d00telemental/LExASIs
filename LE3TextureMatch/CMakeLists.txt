set (PROJ_TARGET LE3TextureMatch)
set (PROJ_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

set (PROJ_SOURCES
  ${PROJ_FOLDER}/Entry.cpp
  ${PROJ_FOLDER}/Entry.hpp
  ${PROJ_FOLDER}/Hooks.cpp
  ${PROJ_FOLDER}/Hooks.hpp
  ${PROJ_FOLDER}/Loading.cpp
  ${PROJ_FOLDER}/Loading.hpp
  ${PROJ_FOLDER}/Manifest.cpp
  ${PROJ_FOLDER}/Manifest.hpp
)

add_library (${PROJ_TARGET} SHARED ${PROJ_SOURCES})
source_group (TREE "${PROJ_FOLDER}" FILES ${PROJ_SOURCES})

target_compile_definitions (${PROJ_TARGET} PUBLIC ${LEASI_COMPILE_DEFINITIONS})
target_compile_options (${PROJ_TARGET} PUBLIC ${LEASI_COMPILE_OPTIONS})
target_include_directories (${PROJ_TARGET} PUBLIC ${PROJ_FOLDER}/..)
target_link_libraries (${PROJ_TARGET} PUBLIC "LExCommon" "LESDK-LE3")
target_link_options (${PROJ_TARGET} PUBLIC ${LEASI_LINK_OPTIONS})

set_target_properties (${PROJ_TARGET} PROPERTIES SUFFIX ".asi")

if (DEFINED ENV{LEASI_GAME3_WIN64})
  set (LEASI_GAME3_WIN64 "$ENV{LEASI_GAME3_WIN64}")
  message ("-- User-provided Mass Effect 3 (LE) Win64 folder: ${LEASI_GAME3_WIN64}")

  set (COPY_SOURCE $<TARGET_FILE:${PROJ_TARGET}>)
  set (COPY_DEST ${LEASI_GAME3_WIN64}/ASI/$<TARGET_FILE_NAME:${PROJ_TARGET}>)

  add_custom_command (TARGET ${PROJ_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${COPY_SOURCE} ${COPY_DEST}
  )

  set_target_properties (${PROJ_TARGET} PROPERTIES
    VS_DEBUGGER_COMMAND "${LEASI_GAME3_WIN64}/MassEffect3.exe"
    VS_DEBUGGER_COMMAND_ARGUMENTS "-enableminidumps -nosplash"
    VS_DEBUGGER_WORKING_DIRECTORY "${LEASI_GAME3_WIN64}"
  )
endif ()
