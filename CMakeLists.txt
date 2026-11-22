# wfc-brun71: the BASIC PDS 7.1 text-mode runtime (BRUN71) for WFC.
#
# The text screen of SCREEN 0 (src/screen.cpp), the keyboard of INKEY$
# (src/keyboard.cpp), code page 437 (src/cp437.cpp), the line editor of INPUT
# (src/input.cpp), PRINT USING (src/using.cpp), SOUND and PLAY
# (src/sound.cpp), the soft keys (src/keyline.cpp), key trapping
# (src/keytrap.cpp) and, on Windows, the console
# host that shows the screen and reads the keys (src/console.cpp).
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

add_library(
  wfc_brun71
  STATIC
  src/screen.cpp
  src/keyboard.cpp
  src/cp437.cpp
  src/input.cpp
  src/using.cpp
  src/sound.cpp
  src/keyline.cpp
  src/keytrap.cpp
)
if(WIN32)
  target_sources(wfc_brun71 PRIVATE src/console.cpp)
  # Users of the library test this before they use console.hpp.
  target_compile_definitions(wfc_brun71 PUBLIC WFC_BRUN71_HAS_CONSOLE)
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
  wfc_brun71_test(TC-0006 input tests/input_test.cpp "unit;brun71")
  wfc_brun71_test(TC-0007 using tests/using_test.cpp "unit;brun71")
  wfc_brun71_test(TC-0008 sound tests/sound_test.cpp "unit;brun71")
  wfc_brun71_test(TC-0009 keyline tests/keyline_test.cpp "unit;brun71")
  wfc_brun71_test(TC-0010 keytrap tests/keytrap_test.cpp "unit;brun71")
  wfc_brun71_test(TC-0011 error tests/error_test.cpp "unit;brun71")
  if(WIN32)
    wfc_brun71_test(TC-0004 console tests/console_test.cpp "integration;brun71")
  endif()
endif()

# TC-0005 runs the PDS corpus programs (tests/corpus) with WFC, whose path is
# WFC_EXECUTABLE, in the interpreter and as native code; without it the tests
# are skipped. Inside WFC the same programs run under WFC's own names.
if(
  BUILD_TESTING
  AND WFC_BUILD_TESTS
  AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR
)
  set(
    WFC_EXECUTABLE
    ""
    CACHE FILEPATH
    "wfc.exe that runs the corpus test case TC-0005 (empty: skipped)"
  )
  file(
    GLOB corpus_expected
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/corpus/*.expected"
  )
  foreach(expected_file IN LISTS corpus_expected)
    get_filename_component(corpus_name "${expected_file}" NAME_WE)
    set(corpus_base "${CMAKE_CURRENT_SOURCE_DIR}/tests/corpus/${corpus_name}")
    set(corpus_keys "")
    if(EXISTS "${corpus_base}.keys")
      set(corpus_keys "-DKEYS=${corpus_base}.keys")
    endif()
    foreach(corpus_mode interpret native)
      set(test_name TC-0005-${corpus_name})
      set(native_option "")
      if(corpus_mode STREQUAL "native")
        set(test_name ${test_name}-native)
        set(native_option -DNATIVE=1)
      endif()
      add_test(
        NAME ${test_name}
        COMMAND
          "${CMAKE_COMMAND}" "-DWFC=${WFC_EXECUTABLE}"
          "-DINPUT=${corpus_base}.bas" "-DEXPECTED=${expected_file}"
          ${corpus_keys} ${native_option} -P
          "${CMAKE_CURRENT_SOURCE_DIR}/tests/corpus/run_corpus_case.cmake"
      )
      set_tests_properties(
        ${test_name}
        PROPERTIES
          LABELS "integration;brun71;corpus"
          TIMEOUT 120
          SKIP_REGULAR_EXPRESSION "SKIPPED:"
      )
    endforeach()
  endforeach()
endif()
