# Source files
libsichSrcs = files(
  'src1.c',
  'src2.c',
  'version.c',
)

# Include directory set for libsich
libsichInc  = include_directories('.')


# Recurse down subdirectories
subdir('util')


# Build library
libsichIncs          = [libsichInc, publicInc]
libsichDeps          = [libm, threads, openmpi]
libsichCArgs         = ['-DSICH_BUILDING_LIBSICH']
libsichLdArgs        = []

if get_option('default_library') == 'shared'
  libsichCArgs += ['-DSICH_SHAREDOBJECT']
  if build_machine.system() == 'windows'
    libsichCArgs += ['-DSICH_BUILDING_DLL']
  else
    libsichCArgs += ['-fvisibility=hidden']
  endif
endif


libsich = library('sich', libsichSrcs,
                  include_directories: libsichIncs,
                  dependencies:        libsichDeps,
                  c_args:              libsichCArgs,
                  link_args:           libsichLdArgs,
                  install:             true,
                  extra_files:         libsichGeneratedHeaders,
                  version:             libsichVersionShort)

#
# Various other targets might want to add an rpath to the install location of
# the library in order for an executable to run without hacks to the library
# PATH. We compute it here.
#
# Also, test executables may want to bypass symbol visibility restrictions;
# This can be done by bursting the library into its component objects and
# relinking those objects directly into the test executables.
#

libsichRpath    = join_paths(get_option('prefix'), get_option('libdir'))
libsichObjs     = libsich.extract_all_objects()

