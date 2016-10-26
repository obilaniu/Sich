# The Sich Project
project('libsich', ['c', 'cpp'],
        version:                 '0.0.0-0',
        meson_version:           '>=0.36.0',
        default_options:         ['warning_level=3',
                                  'strip=true'])


#
# Get the configuration options.
#

buildTests           = get_option('BUILD_TESTS')
buildPerfs           = get_option('BUILD_PERFS')
gtestPath            = get_option('GTEST_PATH')
if gtestPath == ''
  gtestPath          = join_paths('3rdparty', 'googletest')
endif
SOURCE_DATE_EPOCH    = get_option('SOURCE_DATE_EPOCH')


#
# Immediate sanity checks
#
# 1. The build directory and install directories may not point to the same
#    place, because of wierd interactions within Meson if they do.
if (meson.build_root()  == get_option('prefix') or
    meson.source_root() == get_option('prefix'))
  error('Install prefix points to build or source directory! This configuration ' + \
        'is not supported!\n                   Both point to \'' + \
        get_option('prefix') + '\'')
endif


#
# Determine this build's time and complete versioning information by invoking
#     scripts/semverbuildmetadata.py
#
semverScript = join_paths('scripts','semverbuildmetadata.py')
semverCheck  = run_command(semverScript, [meson.source_root(), meson.project_version(),
                           SOURCE_DATE_EPOCH])
semverCheckExitCode = semverCheck.returncode()
if   semverCheckExitCode == 1
  message(semverCheck.stderr())
  error('Insane nominal build time SOURCE_DATE_EPOCH! Must be non-negative '+
        'integer or "now"!')
elif semverCheckExitCode == 2
  error('Project version not compliant with Semantic Version Specification 2.0.0!')
endif
semverCheckList = semverCheck.stdout().strip().split('\n')
if   semverCheckList.length() != 14
  error('Cannot parse output of '+semverScript+'. Did you change it?')
endif

libsichVersionMajor       = semverCheckList[ 0]
libsichVersionMinor       = semverCheckList[ 1]
libsichVersionPatch       = semverCheckList[ 2]
libsichVersionShort       = semverCheckList[ 3]
libsichVersionNormal      = semverCheckList[ 4]
libsichVersionFull        = semverCheckList[ 5]
libsichVersionIsRelease   = semverCheckList[ 6] != '0'
libsichVCSId              = semverCheckList[ 7]
libsichVCSCommit          = semverCheckList[ 8]
libsichVCSClean           = semverCheckList[ 9] != '0'
SOURCE_DATE_EPOCH         = semverCheckList[10]
libsichBuildTimeLocal     = semverCheckList[11]
libsichBuildTimeUTC       = semverCheckList[12]
libsichBuildTimeUTCSemVer = semverCheckList[13]



#
# Print beginning-of-configuration banner.
#
if libsichVCSClean
  headerFooter = '*****************************************************'
else
  headerFooter = '************* WORKING DIRECTORY UNCLEAN *************'
endif

message(headerFooter)
message('Sich     Build Version      '+libsichVersionFull)
message('Nominal  Build Time (Epoch) '+SOURCE_DATE_EPOCH)
message('Nominal  Build Time (UTC)   '+libsichBuildTimeUTC)
message('Nominal  Build Time (Local) '+libsichBuildTimeLocal)
if SOURCE_DATE_EPOCH == '0'
  message('')
  message('NOTE: By default Sich is built reproducibly, with a default')
  message('      timestamp of 0 seconds since the Epoch. This can be changed using')
  message('      the build option')
  message('         -DSOURCE_DATE_EPOCH=<int>')
  message('      or')
  message('         -DSOURCE_DATE_EPOCH=now')
  message('')
endif



#
# With each major version, our headers will go into a different subdirectory
# under include/. For major versions 0 and 1 this will be sich/, but for major
# versions 2 and up we append an integer to produce sich<n>.
#
# This is inspired from OpenCV, which has an old v1 API and a new v2 API,
# respectively under cv/cv*.h and opencv2/*.h
if libsichVersionMajor == '0' or libsichVersionMajor == '1'
  libsichSichSubdir = 'sich'
else
  libsichSichSubdir = 'sich'+libsichVersionMajor
endif


#
# Collect dependencies and build 3rd-party software.
# Depending on what we are able to find, libsich will be configured with or
# without certain features.
subdir('3rdparty')


# The configuration of libsich can now be assembled.
CFG = configuration_data()
CFG.set  ('libsichVersionMajor',              libsichVersionMajor)
CFG.set  ('libsichVersionMinor',              libsichVersionMinor)
CFG.set  ('libsichVersionPatch',              libsichVersionPatch)
CFG.set  ('libsichVersionShort',              libsichVersionShort)
CFG.set  ('libsichVersionNormal',             libsichVersionNormal)
CFG.set  ('libsichVersionFull',               libsichVersionFull)
CFG.set10('libsichVersionIsRelease',          libsichVersionIsRelease)
CFG.set  ('SOURCE_DATE_EPOCH',                SOURCE_DATE_EPOCH)
CFG.set10('haveMPI',                          haveMPI)
CFG.set  ('ccId',                             ccId)
CFG.set  ('cxxId',                            cxxId)
CFG.set  ('ccVersion',                        ccVersion)
CFG.set  ('cxxVersion',                       cxxVersion)
CFG.set  ('libsichVCSId',                     libsichVCSId)
CFG.set  ('libsichVCSCommit',                 libsichVCSCommit)
CFG.set10('libsichVCSClean',                  libsichVCSClean)



# Libraries
subdir('include')
subdir('src')

# Tests
subdir('tests')

# Performance Benchmarks
subdir('perfs')



#
# Print end-of-configuration banner.
#
message(headerFooter)

