/* Include Guard */
#ifndef INCLUDE_SICH_DEPRECATED_H
#define INCLUDE_SICH_DEPRECATED_H


/**
 * This header contains deprecated declaration that are scheduled for removal.
 * In agreement with the Semantic Versioning Specification, deprecated
 * functionality will be marked as such for the duration of at least one minor
 * release, and will be deleted as of the next major release.
 * 
 * Some compilers, such as GCC, support tagging of functions as deprecated
 * and emitting warnings when they are used. When those compilers are in use,
 * deprecated APIs are tagged as such using SICH_DEPRECATED.
 */

#ifdef SICH_BUILDING_LIBSICH
# define SICH_DEPRECATED
#else
# warning "You are using deprecated functionality. This functionality will be removed in the next major version of libsich."
# if __GNUC__ >= 4
#  define SICH_DEPRECATED __attribute__((deprecated))
# else
#  define SICH_DEPRECATED
# endif
#endif

/**
 * Otherwise, this header has a similar organization to all other headers.
 * When declarations in other headers are to be marked deprecated, they shall
 * be moved here, and shall be tagged with SICH_DEPRECATED.
 */


/* Includes */
#include "sich/sich.h"


/* Deprecated Defines */



/* Extern "C" Guard */
#ifdef __cplusplus
extern "C" {
#endif


/**
 * Deprecated Public Data Structures
 */


/**
 * Deprecated Public Global Variables
 */


/**
 * Deprecated Public Function Prototypes
 */

/* SICH_DEPRECATED SICH_PUBLIC ... (...); */



/* End Extern "C" and Include Guard */
#ifdef __cplusplus
}
#endif
#endif

