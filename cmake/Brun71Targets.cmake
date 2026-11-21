# Helpers of the wfc-brun71 targets.

# The C++ standard, the strict warnings and the WSP hardening of a target (the
# last two when the including project defines them).
function(wfc_brun71_target target)
  target_compile_features(${target} PRIVATE cxx_std_20)
  if(COMMAND wfc_enable_strict_warnings)
    wfc_enable_strict_warnings(${target})
  endif()
  if(COMMAND wsp_enable_hardening)
    wsp_enable_hardening(${target})
  endif()
endfunction()

# One test program per test case: <id>-<name> with the library prefix
# (WFC_BRUN71_TEST_PREFIX). Exit status 77 means skipped.
function(wfc_brun71_test id name source labels)
  add_executable(brun71_${name}_test ${source})
  target_link_libraries(brun71_${name}_test PRIVATE wfc_brun71)
  wfc_brun71_target(brun71_${name}_test)
  set(test_name ${WFC_BRUN71_TEST_PREFIX}${id}-${name})
  add_test(NAME ${test_name} COMMAND brun71_${name}_test)
  set_tests_properties(
    ${test_name}
    PROPERTIES LABELS "${labels}" TIMEOUT 60 SKIP_RETURN_CODE 77
  )
endfunction()
