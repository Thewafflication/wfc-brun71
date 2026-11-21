# wfc-brun71: the BASIC PDS 7.1 text-mode runtime (BRUN71) for WFC.
#
# The text screen of SCREEN 0 (src/screen.cpp), the keyboard of INKEY$
# (src/keyboard.cpp), code page 437 (src/cp437.cpp) and, on Windows, the
# console host that shows the screen and reads the keys (src/console.cpp).
# The library is static; the version is the one in VERSION.
#
# Parent project functions used when present: wfc_enable_strict_warnings and
# wsp_enable_hardening; WFC_BRUN71_TEST_PREFIX (see below).

# Standalone build: the library outside WFC. WFC itself adds this directory
# with add_subdirectory and supplies the project, CTest and the WSP hardening.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  cmake_minimum_required(VERSION 3.28)
  project(wfc-brun71 LANGUAGES CXX)
  option(WFC_BUILD_TESTS "Build the tests" ON)
  include(CTest)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  include(WfcWarnings)
  # WSP-SEC-0015: the pinned WSP baseline supplies the hardening flags.
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/wsp/tools/cmake/WspBuild.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/wsp/tools/cmake/WspBuild.cmake")
  endif()
endif()

# CTest names: TC-NNNN-<name> in this repository, wfc-brun71-TC-NNNN-<name>
# when another project mounts the library, so the names of different
# libraries never collide. The including project may set the prefix first.
if(NOT DEFINED WFC_BRUN71_TEST_PREFIX)
  if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(WFC_BRUN71_TEST_PREFIX "")
  else()
    set(WFC_BRUN71_TEST_PREFIX "wfc-brun71-")
  endif()
endif()

include("${CMAKE_CURRENT_LIST_DIR}/cmake/Brun71Targets.cmake")

file(
  STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/VERSION"
  WFC_BRUN71_VERSION
  LIMIT_COUNT 1
)
set(
  WFC_BRUN71_VERSION
  "${WFC_BRUN71_VERSION}"
  CACHE INTERNAL
  "wfcbrun71 version"
)

add_library(wfc_brun71 STATIC src/screen.cpp src/keyboard.cpp src/cp437.cpp)
if(WIN32)
  target_sources(wfc_brun71 PRIVATE src/console.cpp)
endif()
set_target_properties(wfc_brun71 PROPERTIES OUTPUT_NAME wfcbrun71_static)
target_include_directories(
  wfc_brun71
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_compile_definitions(
  wfc_brun71
  PUBLIC "WFC_BRUN71_VERSION=\"${WFC_BRUN71_VERSION}\""
)
wfc_brun71_target(wfc_brun71)

# --- Tests --------------------------------------------------------------------
# One program per test case; each prints `<name>: ok` and exits 0, or prints
# a FAIL line per failed check and exits 1. The console host test exits 77
# (skipped) when the process has no console.
if(BUILD_TESTING AND WFC_BUILD_TESTS)
  wfc_brun71_test(TC-0001 screen tests/screen_test.cpp "unit;brun71")
  wfc_brun71_test(TC-0002 keyboard tests/keyboard_test.cpp "unit;brun71")
  wfc_brun71_test(TC-0003 cp437 tests/cp437_test.cpp "unit;brun71")
  if(WIN32)
    wfc_brun71_test(TC-0004 console tests/console_test.cpp "integration;brun71")
  endif()
endif()
