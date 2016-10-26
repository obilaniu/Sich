/* Include Guards */
#ifndef INCLUDE_SICH_VERSION_H
#define INCLUDE_SICH_VERSION_H


/**
 * The Sich library versions itself following the
 * 
 *                  Semantic Versioning Specification 2.0.0
 *                    'http://semver.org/spec/v2.0.0.html'
 * 
 * It provides preprocessor macros and functions to identify its version at
 * build-time and runtime.
 * 
 * 
 * - SICH_VERSION_MAJOR, SICH_VERSION_MINOR and SICH_VERSION_PATCH expands to
 *   integers representing the current major, minor and patch version numbers
 *   of the software.
 * 
 * - SICH_VERSION_SHORT expands to the "short" version string, which has the
 *   form
 *                           "major.minor.patch"
 * 
 * - SICH_VERSION_NORMAL expands to the "normal" version string, which has the
 *   form
 *                "major.minor.patch<-prerelease.ids.if.any>"
 *   where the content between angle brackets <> shall not be present in a
 *   release version.
 * 
 * - SICH_VERSION_IS_RELEASE expands to 1 if this is a release version, and to
 *   0 otherwise.
 */

#define SICH_VERSION_MAJOR            @libsichVersionMajor@
#define SICH_VERSION_MINOR            @libsichVersionMinor@
#define SICH_VERSION_PATCH            @libsichVersionPatch@
#define SICH_VERSION_SHORT           "@libsichVersionShort@"
#define SICH_VERSION_NORMAL          "@libsichVersionNormal@"
#define SICH_VERSION_IS_RELEASE       @libsichVersionIsRelease@



/* End Include Guard */
#endif

