/* Include Guards */
#ifndef INCLUDE_SICH_REPRODUCIBILITY_H
#define INCLUDE_SICH_REPRODUCIBILITY_H


/**
 * Reproduciblity is the ability to reliably rebuild an exact copy of software
 * from source. Reproducibility requires determinism in the inputs and in the
 * build system; Consequently various things (notably, timestamps) cannot be
 * used in the source code (because we may want to reproduce the build later,
 * when the time will have changed).
 * 
 * Since this file gathers various non-reproducible information whose use would
 * render the build irreproducible, it may *not* be included in libsich's own
 * source code.
 * 
 * This file will emit a warning to remind users of this.
 */

#ifdef SICH_BUILDING_LIBSICH
#error   "This file contains information whose use renders libsich builds \
irreproducible. It may *NOT* be included from within libsich's own source code."
#else
#warning "This file contains information that renders libsich builds \
irreproducible. It may *NOT* be included from within libsich's own source code."
#endif



/* C   compiler that built libsich. */
#define SICH_CC_ID                   "@ccId@"

/* C++ compiler that built libsich. */
#define SICH_CC_VERSION              "@ccVersion@"

/* C   compiler version that built libsich. */
#define SICH_CXX_ID                  "@cxxId@"

/* C++ compiler version that built libsich. */
#define SICH_CXX_VERSION             "@cxxVersion@"



/* Version Control System (VCS) in use. */
#define SICH_VERSION_VCS_ID          "@libsichVCSId@"

/* VCS commit built. */
#define SICH_VERSION_VCS_COMMIT      "@libsichVCSCommit@"

/**
 * Whether VCS working directory was clean during build. For
 *   - "none", this is always 1.
 *   - "git",  this is 1 if 'git diff --cached --exit-code --quiet' and
 *             'git diff --exit-code --quiet' both reported no uncommitted or
 *             unstaged changes relative to HEAD, and 0 otherwise. In other
 *             words, whether what we're compiling matches HEAD precisely.
 */

#define SICH_VERSION_VCS_CLEAN       @libsichVCSClean@


/**
 * SOURCE_DATE_EPOCH
 * 
 * 'https://reproducible-builds.org/specs/source-date-epoch/'
 * 
 * The SOURCE_DATE_EPOCH environment variable defines the presumed build time
 * of libsich as an integer number of seconds since the Epoch
 * (January 1, 1970, 00:00:00 UTC).
 * 
 * It can be converted to a human-readable date in the user's current locale
 * using ctime(), or on any POSIX-compliant operating system using the command
 *     date -d '@<SOURCE_DATE_EPOCH>'
 * where <SOURCE_DATE_EPOCH> is substituted with its value.
 */

#define SICH_SOURCE_DATE_EPOCH       @SOURCE_DATE_EPOCH@


/**
 * SICH_VERSION_FULL expands to the "normal" version string, which has the form
 *                "major.minor.patch<-prerelease.ids.if.any>+build.metadata"
 * where the content between angle brackets <> shall not be present in a
 * release version.
 */

#define SICH_VERSION_FULL            "@libsichVersionFull@"


/* End Include Guards */
#endif
