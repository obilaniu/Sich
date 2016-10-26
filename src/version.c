/* Includes */
#include <ctype.h>
#include "sich/sich.h"
#include <stdlib.h>
#include <string.h>



/**
 * Defines
 */

/**
 * Data Structure Definitions
 */

/**
 * Global Variables
 */

/**
 * Function Prototypes
 */

/**
 * Static Function Definitions
 */

/**
 * Hidden Function Definitions
 */

/**
 * Public Function Definitions
 */

SICH_PUBLIC int            sichGetVersionMajor (void){
	return SICH_VERSION_MAJOR;
}
SICH_PUBLIC int            sichGetVersionMinor (void){
	return SICH_VERSION_MINOR;
}
SICH_PUBLIC int            sichGetVersionPatch (void){
	return SICH_VERSION_PATCH;
}
SICH_PUBLIC const char*    sichGetVersionShort (void){
	return SICH_VERSION_SHORT;
}
SICH_PUBLIC const char*    sichGetVersion      (void){
	return SICH_VERSION_NORMAL;
}
SICH_PUBLIC int            sichIsVersionRelease(void){
	return SICH_VERSION_IS_RELEASE;
}
SICH_PUBLIC int            sichCmpVersions     (const char* aVer, const char* bVer){
	unsigned long long aM, am, ap, bM, bm, bp;
	int                aRelease, bRelease, aDone, bDone, aPre, bPre;
	size_t             s, maxS;
	char* a = (char*)aVer, *b = (char*)bVer;
	
	/**
	 * From the Semantic Versioning Specification 2.0.0
	 *  (http://semver.org/spec/v2.0.0.html):
	 *
	 * 11. Precedence refers to how versions are compared to each other when
	 *     ordered. Precedence MUST be calculated by separating the version
	 *     into major, minor, patch and pre-release identifiers in that order
	 *     (Build metadata does not figure into precedence). Precedence is
	 *     determined by the first difference when comparing each of these
	 *     identifiers from left to right as follows: Major, minor, and patch
	 *     versions are always compared numerically. Example: 1.0.0 < 2.0.0 <
	 *     2.1.0 < 2.1.1. When major, minor, and patch are equal, a pre-release
	 *     version has lower precedence than a normal version. Example:
	 *     1.0.0-alpha < 1.0.0. Precedence for two pre-release versions with
	 *     the same major, minor, and patch version MUST be determined by
	 *     comparing each dot separated identifier from left to right until a
	 *     difference is found as follows: identifiers consisting of only
	 *     digits are compared numerically and identifiers with letters or
	 *     hyphens are compared lexically in ASCII sort order. Numeric
	 *     identifiers always have lower precedence than non-numeric
	 *     identifiers. A larger set of pre-release fields has a higher
	 *     precedence than a smaller set, if all of the preceding identifiers
	 *     are equal. Example: 1.0.0-alpha < 1.0.0-alpha.1 < 1.0.0-alpha.beta
	 *     < 1.0.0-beta < 1.0.0-beta.2 < 1.0.0-beta.11 < 1.0.0-rc.1 < 1.0.0.
	 */
	
	/**
	 * Compare (M)ajor, (m)inor, (p)atch numbers.
	 */
	
	aM = strtoull(a+0, &a, 10);
	bM = strtoull(b+0, &b, 10);
	if(aM<bM){return -1;}
	if(aM>bM){return +1;}
	am = strtoull(a+1, &a, 10);
	bm = strtoull(b+1, &b, 10);
	if(am<bm){return -1;}
	if(am>bm){return +1;}
	ap = strtoull(a+1, &a, 10);
	bp = strtoull(b+1, &b, 10);
	if(ap<bp){return -1;}
	if(ap>bp){return +1;}
	
	/**
	 * Have either or both strings ended immediately after the normal version
	 * numbers, with either a NUL or '+' character (after which follows the
	 * ignored build metadata)?
	 * 
	 * Pre-releases rank lower than releases.
	 */
	
	aRelease = *a == '+' || *a == '\0';
	bRelease = *b == '+' || *b == '\0';
	if( aRelease &&  bRelease){return  0;}
	if(!aRelease &&  bRelease){return -1;}
	if( aRelease && !bRelease){return +1;}
	a++;
	b++;
	
	
	/**
	 * Scan all the way to the first pre-release identifier byte-difference
	 * between a and b, or to the end of the string (whether through NUL or
	 * '+'). We check whether both strings are done; If they are both, they
	 * are equal. 
	 * 
	 * Otherwise, scan backwards to the '.' that began that
	 * identifier (or the initial hyphen if the mismatch occurs in the first
	 * identifier), and find out what was the true meaning of that
	 * difference.
	 */
	
	for(s=0; (a[s]==b[s]) && (a[s]!='\0') && (a[s]!='+'); s++){}
	aDone = a[s] == '+' || a[s] == '\0';
	bDone = b[s] == '+' || b[s] == '\0';
	aPre  = a[s] == '.';
	bPre  = b[s] == '.';
	maxS  = s;
	if(aDone && bDone){return 0;}
	for(   ;     (s>0)    &&        (a[s-1] != '.')     ; s--){}
	
	/**
	 * Numeric indentifiers rank lower than alphabetic.
	 */
	
	if      ( isdigit(a[s]) && !isdigit(b[s])){
		return -1;
	}else if(!isdigit(a[s]) &&  isdigit(b[s])){
		return +1;
	}else if( isdigit(a[s]) &&  isdigit(b[s])){
		return strtoull(a+s, 0, 10) < strtoull(b+s, 0, 10) ? -1 : +1;
	}else{
		/**
		 * If we get here, the comparison is lexicographical, the versions are
		 * different and they differ precisely at a[maxS] != b[maxS]. At most
		 * one of the strings is done.
		 * 
		 * One of only a few things can have happened:
		 * 
		 *   - One string is done, the other has more pre-release identifiers.
		 *        >>> The string with more identifiers ranks higher.
		 *   - One string is done, the other's pre-release identifier is longer.
		 *        >>> The shorter string ranks lexicographically lower.
		 *   - One string's pre-release identifier is ended, the other's
		 *     pre-release identifier is longer.
		 *        >>> The shorter string ranks lexicographically lower.
		 *   - Neither string has ended identifiers: They differ in some
		 *     alphanumeric or hyphen character, and not in NUL, '+' or '.'.
		 *        >>> The characters in question are compared ASCIIbetically.
		 */
		
		if( aDone &&  bPre ){return -1;}
		if( bDone &&  aPre ){return +1;}
		
		if( aDone && !bDone){return -1;}
		if(!aDone &&  bDone){return +1;}
		
		if( aPre  && !bPre ){return -1;}
		if( bPre  && !aPre ){return +1;}
		
		return a[maxS]<b[maxS] ? -1 : +1;
	}
}

