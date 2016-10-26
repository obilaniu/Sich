/* Includes */
#include "rng.h"
#include "sich/sich.h"
#include <stdint.h>
#include <stdlib.h>



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
 * Hidden Function Definitions
 */

SICH_HIDDEN void     sichLcg32Init      (SICH_LCG32* S, uint32_t seed){
	S->S = seed;
}
SICH_HIDDEN void     sichLcg64Init      (SICH_LCG64* S, uint64_t seed){
	S->S = seed;
}
SICH_HIDDEN void     sichLcg32Skip      (SICH_LCG32* S, uint32_t n){
	uint32_t a = SICH_LCG32_a,
	         c = SICH_LCG32_c;
	
	while(n){
		if(n&1){
			S->S = a*S->S + c;
		}
		
		c  *= a+1;
		a  *= a;
		n >>= 1;
	}
}
SICH_HIDDEN void     sichLcg64Skip      (SICH_LCG64* S, uint64_t n){
	uint64_t a = SICH_LCG64_a,
	         c = SICH_LCG64_c;
	
	while(n){
		if(n&1){
			S->S = a*S->S + c;
		}
		
		c  *= a+1;
		a  *= a;
		n >>= 1;
	}
}
SICH_HIDDEN uint32_t sichLcg32Dist      (const SICH_LCG32* S){
	uint32_t a = SICH_LCG32_a,
	         c = SICH_LCG32_c,
	         p = 1,
	         Z = 0,
	         D = 0;
	
	while(Z != S->S){
		if((Z^S->S) & p){
			Z  = a*Z + c;
			D += p;
		}
		
		c  *= a+1;
		a  *= a;
		p <<= 1;
	}
	
	return D;
}
SICH_HIDDEN uint64_t sichLcg64Dist      (const SICH_LCG64* S){
	uint64_t a = SICH_LCG64_a,
	         c = SICH_LCG64_c,
	         p = 1,
	         Z = 0,
	         D = 0;
	
	while(Z != S->S){
		if((Z^S->S) & p){
			Z  = a*Z + c;
			D += p;
		}
		
		c  *= a+1;
		a  *= a;
		p <<= 1;
	}
	
	return D;
}
SICH_HIDDEN uint32_t sichLcg32Diff      (const SICH_LCG32* Ss, const SICH_LCG32* Se){
	uint32_t a = SICH_LCG32_a,
	         c = SICH_LCG32_c,
	         p = 1,
	         Z = Ss->S,
	         D = 0;
	
	while(Z != Se->S){
		if((Z^Se->S) & p){
			Z  = a*Z + c;
			D += p;
		}
		
		c  *= a+1;
		a  *= a;
		p <<= 1;
	}
	
	return D;
}
SICH_HIDDEN uint64_t sichLcg64Diff      (const SICH_LCG64* Ss, const SICH_LCG64* Se){
	uint64_t a = SICH_LCG64_a,
	         c = SICH_LCG64_c,
	         p = 1,
	         Z = Ss->S,
	         D = 0;
	
	while(Z != Se->S){
		if((Z^Se->S) & p){
			Z  = a*Z + c;
			D += p;
		}
		
		c  *= a+1;
		a  *= a;
		p <<= 1;
	}
	
	return D;
}
SICH_HIDDEN uint32_t sichLcg32Peek      (const SICH_LCG32* S){
	return S->S;
}
SICH_HIDDEN uint64_t sichLcg64Peek      (const SICH_LCG64* S){
	return S->S;
}
SICH_HIDDEN uint32_t sichPcg32RxsMXsPeek(const SICH_LCG32* S){
	uint32_t tmp = ((S->S >> ((S->S >> 28) + 4)) ^ S->S) * 277803737;
	return tmp ^ (tmp >> 22);
}
SICH_HIDDEN uint32_t sichPcg64XshRsPeek (const SICH_LCG64* S){
	return (S->S ^ (S->S >> 22)) >> (22 + (S->S >> 61));
}
SICH_HIDDEN uint32_t sichLcg32Next      (SICH_LCG32* S){
	S->S = SICH_LCG32_a*S->S + SICH_LCG32_c;
	return sichLcg32Peek(S);
}
SICH_HIDDEN uint64_t sichLcg64Next      (SICH_LCG64* S){
	S->S = SICH_LCG64_a*S->S + SICH_LCG64_c;
	return sichLcg64Peek(S);
}
SICH_HIDDEN uint32_t sichPcg32RxsMXsNext(SICH_LCG32* S){
	S->S = SICH_LCG32_a*S->S + SICH_LCG32_c;
	return sichPcg32RxsMXsPeek(S);
}
SICH_HIDDEN uint32_t sichPcg64XshRsNext (SICH_LCG64* S){
	S->S = SICH_LCG64_a*S->S + SICH_LCG64_c;
	return sichPcg64XshRsPeek(S);
}

