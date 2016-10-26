# Collect dependencies and build 3rd-party software.
# - math       (required) (Standard C Library)
# - theads     (required) (OS-specific)
# - MPI        (optional) (Need mpicc in PATH)
# - GoogleTest (optional) (In googletest/ subdirectory)

haveGTest            = false
haveMPI              = false


# Collect dependencies
cc                   = meson.get_compiler('c')
cxx                  = meson.get_compiler('cpp')
ccId                 = cc.get_id()
cxxId                = cxx.get_id()
ccVersion            = cc.version()
cxxVersion           = cxx.version()
isGCCorClang         = (((ccId  == 'gcc') or (ccId  == 'clang')) and
                        ((cxxId == 'gcc') or (cxxId == 'clang')))
libm                 = cc.find_library('m',          required: true)
threads              = dependency     ('threads',    required: true)
mpicc                = find_program   ('mpicc',      required: false)
haveMPI              = mpicc.found()
if haveMPI
  openmpiCFLAGS  = run_command('mpicc', ['--showme:compile']).stdout().strip().split(' ')
  openmpiLDFLAGS = run_command('mpicc', ['--showme:link'   ]).stdout().strip().split(' ')
else
  openmpiCFLAGS  = []
  openmpiLDFLAGS = []
endif
openmpi = declare_dependency(compile_args: openmpiCFLAGS,
                             link_args:    openmpiLDFLAGS)

#
# Add Global C/C++ Flags.
# 
# Several flags are useful for a secure or reproducible build. In particular,
# using a constant PRNG seed and disabling the build ID make builds much more
# reproducible, while enforcing full read-only GOT relocations prevents certain
# memory exploits.
#
# Stripping debug information is required for a reproducible build.
#

globalCcArgs            = []
globalCxxArgs           = []
globalCcLdArgs          = []
globalCxxLdArgs         = []

# Muzzle compiler complaints about long long
globalCcArgs           += cc .first_supported_argument(['-Wno-long-long'])
globalCxxArgs          += cxx.first_supported_argument(['-Wno-long-long'])
# Reproducible build, random seed 0.
globalCcArgs           += cc .first_supported_argument(['-frandom-seed=0'])
globalCxxArgs          += cxx.first_supported_argument(['-frandom-seed=0'])
# Safety: full read-only relro.
globalCcLdArgs         += cc. first_supported_argument(['-Wl,-z,relro,-z,now'])
globalCxxLdArgs        += cxx.first_supported_argument(['-Wl,-z,relro,-z,now'])
# Reproducible build, build id is problematic
globalCcLdArgs         += cc. first_supported_argument(['-Wl,--build-id=none'])
globalCxxLdArgs        += cxx.first_supported_argument(['-Wl,--build-id=none'])
add_global_arguments     (globalCcArgs,    language: 'c')
add_global_arguments     (globalCxxArgs,   language: 'cpp')
add_global_link_arguments(globalCcLdArgs,  language: 'c')
add_global_link_arguments(globalCxxLdArgs, language: 'cpp')



# Build 3rd-party software

# GoogleTest
# 
# libsich's tests are implemented using GoogleTest. We will only build these
# tests if asked (BUILD_TESTS=true) and if GoogleTest is available in source
# form at the path GTEST_PATH.
#
# If GTEST_PATH is relative, it is taken relative to MESON_SOURCE_ROOT.
# If GTEST_PATH is absolute, it is taken absolutely.
#
# If we want to build tests and we have GoogleTest available, we need to build
# static library gtest_main if it hasn't been done yet.
#

buildTests = get_option('BUILD_TESTS')
if buildTests
  #
  # We handcraft a tiny Python script to check whether the path does in fact
  # point to the source code of Google Tests. Its exit code is 0 if it finds
  # GoogleTest, and on stdout prints the absolute path to the GoogleTest being
  # used.
  #
  
  checkGoogleTestScript = '''
import os,sys
fPath        = '''+'\''+gtestPath+'\''+'''
if not os.path.isabs(fPath):
  fPath        = os.path.join (os.environ['MESON_SOURCE_ROOT'],
                               fPath, 'googletest', 'src', 'gtest-all.cc')
haveGtestAll = os.path.isfile (fPath);
fPath        = os.path.dirname(fPath)
fPath        = os.path.dirname(fPath)
fPath        = os.path.dirname(fPath)
sys.stdout.write(fPath)
sys.exit(0 if haveGtestAll else 1);
'''
  # DEBUG: message(checkForGTest.stderr())
  
  checkForGTest = run_command('python', ['-c', checkGoogleTestScript])
  gtestPath  = checkForGTest.stdout()
  haveGTest  = checkForGTest.returncode() == 0
  if haveGTest
    message('Building with GoogleTest from \''+gtestPath+'\'')
    gtgt       = join_paths(gtestPath, 'googletest')
    gtgtinc    = join_paths(gtgt,      'include')
    gtgtsrc    = join_paths(gtgt,      'src')
    
    libgtest_mainSrcs    = files(join_paths(gtgtsrc, 'gtest-all.cc'),
                                 join_paths(gtgtsrc, 'gtest_main.cc'))
    libgtest_mainInc     = include_directories(gtgt, gtgtinc)
    libgtest_mainIncs    = [libgtest_mainInc]
    libgtest_mainDeps    = [threads]
    libgtest_mainCppArgs = ['-Wno-variadic-macros', '-Wno-pedantic']
    
    libgtest_main        = static_library('gtest_main',        libgtest_mainSrcs,
                                          include_directories: libgtest_mainIncs,
                                          cpp_args:            libgtest_mainCppArgs,
                                          dependencies:        libgtest_mainDeps)
  endif
endif
