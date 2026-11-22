# Runs one PDS corpus program through wfc and compares what it prints with the
# expected text (test case TC-0005, the same check WFC runs for its corpus).
# Without a usable WFC the script prints SKIPPED, which CTest reports as a
# skipped test.
# Usage: cmake -DWFC=<wfc> -DINPUT=<.bas> -DEXPECTED=<file> [-DKEYS=<file>]
#   [-DNATIVE=1] -P run_corpus_case.cmake
# KEYS is the standard input of the program: the keys it reads with INKEY$.
# NATIVE runs the program as native code instead of in the interpreter.
# A <name>.exitcode file beside the program gives the exit status it must
# end with (default 0).
if(NOT WFC OR NOT EXISTS "${WFC}")
  message(STATUS "SKIPPED: set WFC_EXECUTABLE to wfc.exe to run this test")
  return()
endif()
set(mode_option --interpret)
if(NATIVE)
  set(mode_option "")
endif()
set(input_option "")
if(DEFINED KEYS)
  set(input_option INPUT_FILE "${KEYS}")
endif()
# A program that ends on an error a handler does not take has a
# <name>.exitcode file beside it with the exit status wfc gives (1).
get_filename_component(case_dir "${INPUT}" DIRECTORY)
get_filename_component(case_name "${INPUT}" NAME_WE)
set(expected_result 0)
if(EXISTS "${case_dir}/${case_name}.exitcode")
  file(
    STRINGS "${case_dir}/${case_name}.exitcode"
    expected_result
    LIMIT_COUNT 1
  )
endif()
execute_process(
  COMMAND "${WFC}" ${mode_option} "${INPUT}" ${input_option}
  OUTPUT_VARIABLE actual
  ERROR_VARIABLE errors
  RESULT_VARIABLE result
)
if(NOT result EQUAL expected_result)
  message(FATAL_ERROR "wfc exited with ${result}: ${errors}")
endif()
file(READ "${EXPECTED}" expected)
string(REPLACE "\r\n" "\n" expected "${expected}")
string(REPLACE "\r\n" "\n" actual "${actual}")
string(STRIP "${expected}" expected)
string(STRIP "${actual}" actual)
if(NOT actual STREQUAL expected)
  message(
    FATAL_ERROR
    "corpus output mismatch\n--- expected ---\n${expected}\n\
--- actual ---\n${actual}"
  )
endif()
