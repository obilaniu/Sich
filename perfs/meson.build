#
# Performance Benchmarks
#

if buildPerfs
  # perf_sich
  perf_sichSrcs  = files('perf_sich.c')
  perf_sichIncs  = [libsichInc, publicInc]
  
  executable('perf_sich', perf_sichSrcs,
             include_directories: libsichIncs,
             link_with:           [libsich],
             install:             true)
endif
