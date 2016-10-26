/* Include Guards */
#ifndef INCLUDE_SICH_VISIBILITY_H
#define INCLUDE_SICH_VISIBILITY_H


/**
 * When building a library, it is a good idea to expose as few as possible
 * internal symbols (functions, objects, data structures). Not only does it
 * prevent users from relying on private portions of the library that are
 * subject to change without any notice, but it can have performance
 * advantages:
 * 
 *   - It can make shared libraries link faster at dynamic-load time.
 *   - It can make internal function calls faster by bypassing the PLT.
 * 
 * Thus, the compilation will by default hide all symbols, while the API
 * headers will explicitly mark public the few symbols the users are permitted
 * to use with a tag SICH_PUBLIC. We also define a tag SICH_HIDDEN, since it
 * may be required to explicitly tag certain C++ types as visible in order for
 * exceptions to function correctly.
 * 
 * Additional complexity comes from non-POSIX-compliant systems, which
 * artificially impose a requirement on knowing whether we are building or
 * using a DLL.
 * 
 * The above commentary and below code is inspired from
 *                   'https://gcc.gnu.org/wiki/Visibility'
 */

#if    defined(SICH_SHAREDOBJECT) &&  defined(_WIN32) &&  defined(SICH_BUILDING_DLL)
# define SICH_PUBLIC __declspec(dllexport)
# define SICH_HIDDEN
#elif  defined(SICH_SHAREDOBJECT) &&  defined(_WIN32) && !defined(SICH_BUILDING_DLL)
# define SICH_PUBLIC __declspec(dllimport)
# define SICH_HIDDEN
#elif  defined(SICH_SHAREDOBJECT) &&  __GNUC__ >= 4
# define SICH_PUBLIC __attribute__((visibility("default")))
# define SICH_HIDDEN __attribute__((visibility("hidden")))
#else
# define SICH_PUBLIC
# define SICH_HIDDEN
#endif
# define SICH_STATIC static


/* End Include Guards */
#endif
