/* Include Guard */
#ifndef INCLUDE_SICH_SICH_H
#define INCLUDE_SICH_SICH_H


/**
 * Includes.
 * 
 * NOTE:
 *   The headers sich/deprecated.h and sich/reproducibility.h are intentionally
 *   *NOT* included here.
 *   
 *   To use deprecated functionality, you must explicitly
 *   #include "sich/deprecated.h"; This serves to force a client of this
 *   library to acknowledge the impending removal of the functionality.
 *   
 *   The header sich/reproducibility.h isolates information that would cause
 *   the build to become irreproducible. For that reason it may not be included
 *   from within libsich's own source code.
 */

#include "sich/config.h"
#include "sich/version.h"
#include "sich/visibility.h"



/* Defines */




/* Extern "C" Guard */
#ifdef __cplusplus
extern "C" {
#endif


/* Data Structures */
#ifndef SICH_TENSOR_NAME
typedef double T;
#else
typedef double SICH_TENSOR_NAME;
#endif



/* Function Prototypes */

/**
 * @brief  Get Sich Library Version.
 * 
 * The Sich library versions itself according to the
 * 
 *                  Semantic Versioning Specification 2.0.0
 *                    'http://semver.org/spec/v2.0.0.html'
 * 
 * sichGetVersionMajor() returns the major version of the library. The
 * corresponding preprocessor macro is SICH_VERSION_MAJOR.
 * 
 * sichGetVersionMinor() returns the major version of the library. The
 * corresponding preprocessor macro is SICH_VERSION_MINOR.
 * 
 * sichGetVersionPatch() returns the patch version of the library. The
 * corresponding preprocessor macro is SICH_VERSION_PATCH.
 * 
 * sichGetVersion() returns a normal version string, in the form
 * "major.minor.patch<-prerelease.identifiers.if.any>". The corresponding
 * preprocessor macro is SICH_VERSION_NORMAL.
 * 
 * sichGetVersionShort() returns a short version string in the form
 * "major.minor.patch". The corresponding preprocessor macro is
 * SICH_VERSION_SHORT.
 * 
 * sichIsVersionRelease() returns whether (1) or not (0) the version is
 * a release version. The corresponding preprocessor macro is
 * SICH_VERSION_IS_RELEASE.
 * 
 * @returns A version string (sichGetVersion, sichGetVersionShort),
 *          a portion of it as an integer (sichGetVersionMajor,
 *          sichGetVersionMinor, sichGetVersionPatch), or a boolean
 *          (sichIsRelease).
 */

SICH_PUBLIC int            sichGetVersionMajor (void);
SICH_PUBLIC int            sichGetVersionMinor (void);
SICH_PUBLIC int            sichGetVersionPatch (void);
SICH_PUBLIC const char*    sichGetVersion      (void);
SICH_PUBLIC const char*    sichGetVersionShort (void);
SICH_PUBLIC int            sichIsVersionRelease(void);

/**
 * @brief Compare version strings using semantic version comparison algorithm.
 * @param [in]     a  First  version string
 * @param [in]     b  Second version string
 * @returns -1 if a<b, 0 if a==b and 1 if a>b, in agreement with
 * 
 *                  Semantic Versioning Specification 2.0.0
 *                    'http://semver.org/spec/v2.0.0.html'
 */

SICH_PUBLIC int            sichCmpVersions     (const char* aVer, const char* bVer);



/* End Extern "C" and Include Guard */
#ifdef __cplusplus
}
#endif
#endif
