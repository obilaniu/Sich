/* Include Guard */
#ifndef SRC_RNG_H
#define SRC_RNG_H

/**
 * The PCG PRNG (pcg-random.org)
 * 
 * PCG (Permuted Congruential Generator) is a fast, high-quality pseudo-random
 * number generator (PRNG). It is based on an earlier family of PRNGs called
 * LCG (Linear Congruential Generators), but adds an improvement to mask their
 * well-known statistical flaws: An output function that masks the output.
 * 
 * 
 * LCGs are extremely simple. They contain a state (S) consisting in a single
 * integer, usually 32 or 64 bits. Their state transition is simply
 *               S = a*S + c (mod m)
 * , with a, c, m integers. Their output O is equal to the new state S.
 * 
 * By the Hull-Dobell Theorem, LCGs produce longest-period output iff:
 *   1) m and c are coprime.
 *   2) a-1 is divisible by all prime factors of m.
 *   3) a-1 is divisible by 4 if m is divisible by 4.
 * The fastest LCGs avoid the slow modulo-m operation by exploiting truncation
 * mod 2^32 or mod 2^64, essentially getting modular arithmetic for free using
 * the correct word size. PCG uses this trick.
 * 
 * when an LCG uses m = 2^{word size}, where the word size is usually 32 or
 * 64 bits, and m is a pure power of 2 divisible by 4, it follows that by the
 * Hull-Dobell Theorem, the following should be the case:
 *   1) c is odd.
 *   2) a is odd.
 *   3) a is congruent to 1 modulo 4.
 * 3) is a strictly stronger condition than 2), so the parameters are:
 *   - Modulus    m = 2^{word size}
 *   - Increment  c = 1 mod 2 (c is odd)
 *   - Multiplier a = 1 mod 4
 * 
 * 
 * But all LCGs, by themselves, are known to have serious statistical flaws;
 * Specifically, their state (S), and thus their output (O), is very obviously
 * distributed in organized, high-dimensional hyperplanes. This is the problem
 * that PCG solves, by breaking the direct connection O=S. It does so by
 * applying an output function O=f(S). f() can be anything, but a good choice
 * of f() can greatly improve quality while a poor choice can much decrease it.
 * Specifically, f() should be a permutation function, and when its output is
 * the same size as the state, it will be a 1-to-1 function.
 * 
 * Usually, f() will involve data-dependent xorshifts (x ^= x >> k), adds and
 * bit-rotates in an attempt at scrambling S. The following PCG variants are
 * implemented here:
 * 
 * - 32-bit Output, 32-bit State: PCG32-RXS-M-XS
 *     a   = 747796405
 *     c   = 2891336453
 *     S   = a*S + c
 *     T   = ((S >> ((S >> 28) + 4)) ^ S) * 277803737;
 *     O   = T ^ (T >> 22)
 * - 32-bit Output, 64-bit State: PCG64-XSH-RS
 *     a   = 6364136223846793005
 *     c   = 1442695040888963407
 *     S   = a*S + c
 *     O   = (S ^ (S >> 22)) >> (22 + (S >> 61))
 */



/* Includes */
#include "sich/sich.h"
#include <stdint.h>


/**
 * LCG parameters.
 */

#define SICH_LCG32_a                                      747796405ull
#define SICH_LCG32_c                                     2891336453ull
#define SICH_LCG64_a                            6364136223846793005ull
#define SICH_LCG64_c                            1442695040888963407ull
#define SICH_INV_2_POW_64      5.4210108624275221700372640043497086e-20l
#define SICH_INV_2_POW_64M1    5.4210108624275221706250111797608523e-20l
#define SICH_INV_2_POW_32      2.3283064365386962890625000000000000e-10l
#define SICH_INV_2_POW_32M1    2.3283064370807973755576877066744502e-10l



/* Extern "C" Guard */
#ifdef __cplusplus
extern "C" {
#endif



/* Data Structures */

/**
 * 32-bit PCG state.
 */

struct SICH_LCG32{
	uint32_t S;
};
typedef struct SICH_LCG32 SICH_LCG32;

/**
 * 64-bit PCG state.
 */

struct SICH_LCG64{
	uint64_t S;
};
typedef struct SICH_LCG64 SICH_LCG64;



/* Function Prototypes */

/**
 * Initialize an LCG state with a seed.
 * 
 * @param [out]     S     A 32-bit or 64-bit state to initialize.
 * @param [in]      seed  A 32-bit or 64-bit seed.
 */

SICH_HIDDEN void     sichLcg32Init      (SICH_LCG32* S, uint32_t seed);
SICH_HIDDEN void     sichLcg64Init      (SICH_LCG64* S, uint64_t seed);

/**
 * Skip over n random number generation calls in an efficient, O(log n) manner.
 * 
 * @param [in,out]  S  An LCG state to be skipped ahead
 * @param [in]      n  The number of calls to be skipped.
 */

SICH_HIDDEN void     sichLcg32Skip      (SICH_LCG32* S, uint32_t n);
SICH_HIDDEN void     sichLcg64Skip      (SICH_LCG64* S, uint64_t n);

/**
 * Compute state's "distance" from state 0.
 * 
 * This corresponds to the number of calls to Next() it would take to get from
 * a state of 0 to the current state.
 * 
 * @param [in]      S  A state whose distance from 0 is to be computed.
 * @return          The distance of state S from 0.
 */

SICH_HIDDEN uint32_t sichLcg32Dist      (const SICH_LCG32* S);
SICH_HIDDEN uint64_t sichLcg64Dist      (const SICH_LCG64* S);

/**
 * Compute difference between two states.
 * 
 * This corresponds to the number of calls to Next() it would take to get from
 * the first state to the second state.
 * 
 * @param [in]      Ss  The state from which we start.
 * @param [in]      Se  The state to   which we go.
 * @return          The number of states from Ss to Se.
 */

SICH_HIDDEN uint32_t sichLcg32Diff      (const SICH_LCG32* Ss, const SICH_LCG32* Se);
SICH_HIDDEN uint64_t sichLcg64Diff      (const SICH_LCG64* Ss, const SICH_LCG64* Se);

/**
 * Peek at the current random number output *without* altering the state.
 * 
 * @param [in]      S  A state whose current output is to be read.
 * @return          The current output.
 */

SICH_HIDDEN uint32_t sichLcg32Peek      (const SICH_LCG32* S);
SICH_HIDDEN uint64_t sichLcg64Peek      (const SICH_LCG64* S);
SICH_HIDDEN uint32_t sichPcg32RxsMXsPeek(const SICH_LCG32* S);
SICH_HIDDEN uint32_t sichPcg64XshRsPeek (const SICH_LCG64* S);

/**
 * Get the next random number output, advancing the state one step.
 * 
 * @param [in]      S  A state that should be advanced one step and whose next
 *                     output is afterwards to be read.
 * @return          The next output.
 */

SICH_HIDDEN uint32_t sichLcg32Next      (SICH_LCG32* S);
SICH_HIDDEN uint64_t sichLcg64Next      (SICH_LCG64* S);
SICH_HIDDEN uint32_t sichPcg32RxsMXsNext(SICH_LCG32* S);
SICH_HIDDEN uint32_t sichPcg64XshRsNext (SICH_LCG64* S);


/* End Extern "C" and Include Guard */
#ifdef __cplusplus
}
#endif
#endif
