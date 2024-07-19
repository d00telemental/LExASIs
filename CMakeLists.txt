cmake_minimum_required (VERSION 3.27)
project (LExASIs VERSION 0.1.0 LANGUAGES CXX)


# ! Global configuration.
# ========================================

cmake_policy (SET CMP0057 NEW)
cmake_policy (SET CMP0077 NEW)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set_property (GLOBAL PROPERTY USER_FOLDERS ON)

if (DEFINED ENV{LEASI_CMAKE_PRESET})
  set (LEASI_CMAKE_PRESET $ENV{LEASI_CMAKE_PRESET})
  message ("-- Environment selected preset: ${LEASI_CMAKE_PRESET}")
else ()
  message (FATAL_ERROR "!! Environment does not select preset via LEASI_CMAKE_PRESET")
endif ()


# ! Build configuration.
# ========================================

if (CMAKE_GENERATOR MATCHES "Visual Studio 17 2022")
  set (LEASI_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build/$<CONFIG>)
  message ("-- Detected supported generator: ${CMAKE_GENERATOR}")
  message ("-- Configured output path: ${LEASI_OUTPUT_DIRECTORY}")
else ()
  message (FATAL_ERROR "!! Detected unsupported generator: ${CMAKE_GENERATOR}")
endif ()

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LEASI_OUTPUT_DIRECTORY})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LEASI_OUTPUT_DIRECTORY})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LEASI_OUTPUT_DIRECTORY})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND WIN32)

  add_compile_definitions ( "_CRT_SECURE_NO_WARNINGS" "NOMINMAX" )

  add_compile_options ( "/MP" "/utf-8" "/Zi"
    "$<$<IN_LIST:$<CONFIG>,DEBUG>:/MDd;/Od;/JMC>"
    "$<$<IN_LIST:$<CONFIG>,RELEASE>:/MD;/O2>"
  )

  add_link_options ( "/DEBUG"
    "$<$<IN_LIST:$<CONFIG>,DEBUG>:/OPT:NOREF;/OPT:NOICF>"
    "$<$<IN_LIST:$<CONFIG>,RELEASE>:/OPT:REF;/OPT:ICF;/INCREMENTAL:NO>"
  )

  set (LEASI_COMPILE_DEFINITIONS
    "SPDLOG_ACTIVE_LEVEL=0"  # Log TRACE and up.
    "$<$<IN_LIST:$<CONFIG>,DEBUG>:_DEBUG>"
  )

  set (LEASI_COMPILE_OPTIONS
    "/W4" "/WX" "/permissive"
    "/wd4121"  # -warn: alignment of member was sensitive to padding
    "/wd4127"  # -warn: conditional expression is constant
  )

  set (LEASI_LINK_OPTIONS
    # ...
  )

else ()

  # Note that compiler conditions are explicitly paired with host system checks.
  message (FATAL_ERROR "!! Unsupported compiler ${CMAKE_CXX_COMPILER_ID} on ${CMAKE_SYSTEM_NAME}")

endif ()


# ! Project targets.
# ========================================

add_subdirectory ("External")
add_subdirectory ("LExCommon")
add_subdirectory ("LE3TextureOverride")

set_property (DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "LE3TextureOverride")

set (REPO_SOURCES
  .gitignore
  .gitmodules
  CMakeLists.txt
  CMakePresets.json
)

add_custom_target (Repository SOURCES ${REPO_SOURCES})
source_group (TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${REPO_SOURCES})
