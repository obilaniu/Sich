#
# TESTS
#

if buildTests and haveGTest
  #
  # If we have GoogleTest, we compile the testsuite. To do this we have
  # beforehand "burst" the compiled library libsich into its constituent object
  # files (libsichObjs). Now, we relink these objects against the test files,
  # thus dodging symbol visibility issues.
  # 
  # It does mean we must manage by hand libsich's dependencies and flags.
  #
  
  # test_sich
  test_sichSrcs  = files('test_sich.cpp')
  test_sichIncs  = [libgtest_mainInc, libsichInc, publicInc]
  
  executable('test_sich', test_sichSrcs,
             include_directories: test_sichIncs,
             cpp_args:            libgtest_mainCppArgs,
             link_with:           [libgtest_main],
             objects:             [libsichObjs],
             dependencies:        [libgtest_mainDeps, libsichDeps],
             install:             true)
endif
