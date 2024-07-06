set (PROJ_TARGET LExCommon)
set (PROJ_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

set (PROJ_SOURCES
  ${PROJ_FOLDER}/Base.cpp
  ${PROJ_FOLDER}/Base.hpp
)

add_library (${PROJ_TARGET} STATIC ${PROJ_SOURCES})
source_group (TREE "${PROJ_FOLDER}" FILES ${PROJ_SOURCES})

target_compile_definitions (${PROJ_TARGET} PUBLIC ${LEASI_COMPILE_DEFINITIONS})
target_compile_options (${PROJ_TARGET} PUBLIC ${LEASI_COMPILE_OPTIONS})
target_include_directories (${PROJ_TARGET} PUBLIC ${PROJ_FOLDER}/..)
target_link_libraries (${PROJ_TARGET} PUBLIC "spdlog")
target_link_options (${PROJ_TARGET} PUBLIC ${LEASI_LINK_OPTIONS})
