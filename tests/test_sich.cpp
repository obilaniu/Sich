#include <gtest/gtest.h>
#include "sich/sich.h"
#include "util/rng.h"
#include <stdlib.h>

#if 0
TEST(TestCase, Test){
	/**
	 * ASSERT_* macros are fatal if failed.
	 * EXPECT_* macros are not fatal if failed.
	 */
	
	ASSERT_TRUE(true);/* Fatal     if failed */
	EXPECT_TRUE(true);/* Non-fatal if failed */
	EXPECT_EQ(0,0);/* Non-fatal if failed */
	EXPECT_NE(0,1);/* Non-fatal if failed */
	EXPECT_LT(0,1);/* Non-fatal if failed */
	EXPECT_LE(0,1);/* Non-fatal if failed */
	EXPECT_GT(1,0);/* Non-fatal if failed */
	EXPECT_GE(1,0);/* Non-fatal if failed */
}
#endif

TEST(SemanticVersioning, DifferentNormalVersionNumbers){
	EXPECT_EQ(sichCmpVersions("1.0.0", "1.0.0"),  0);
	EXPECT_EQ(sichCmpVersions("2.0.0", "2.0.0"),  0);
	EXPECT_EQ(sichCmpVersions("2.1.0", "2.1.0"),  0);
	EXPECT_EQ(sichCmpVersions("2.1.1", "2.1.1"),  0);
	
	EXPECT_LT(sichCmpVersions("1.0.0", "2.0.0"),  0);
	EXPECT_LE(sichCmpVersions("1.0.0", "2.0.0"),  0);
	EXPECT_GT(sichCmpVersions("2.0.0", "1.0.0"),  0);
	EXPECT_GE(sichCmpVersions("2.0.0", "1.0.0"),  0);
	EXPECT_EQ(sichCmpVersions("1.0.0", "2.0.0"), -1);
	EXPECT_EQ(sichCmpVersions("2.0.0", "1.0.0"), +1);
	
	EXPECT_LT(sichCmpVersions("1.0.0", "2.0.0"),  0);
	EXPECT_LT(sichCmpVersions("1.0.0", "2.1.0"),  0);
	EXPECT_LT(sichCmpVersions("1.0.0", "2.1.1"),  0);
	
	EXPECT_LT(sichCmpVersions("2.0.0", "2.1.0"),  0);
	EXPECT_LT(sichCmpVersions("2.0.0", "2.1.1"),  0);
	
	EXPECT_LT(sichCmpVersions("2.1.0", "2.1.1"),  0);
}
TEST(SemanticVersioning, PrereleaseVersionNumbers){
	EXPECT_EQ(sichCmpVersions("1.0.0-alpha", "1.0.0"),  -1);
}
TEST(SemanticVersioning, SameNormalVersionNumbers){
	EXPECT_EQ(sichCmpVersions("1.0.0-alpha",      "1.0.0-alpha.1"),     -1);
	EXPECT_EQ(sichCmpVersions("1.0.0-alpha.1",    "1.0.0-alpha.beta"),  -1);
	EXPECT_EQ(sichCmpVersions("1.0.0-alpha.beta", "1.0.0-beta"),        -1);
	EXPECT_EQ(sichCmpVersions("1.0.0-beta",       "1.0.0-beta.2"),      -1);
	EXPECT_EQ(sichCmpVersions("1.0.0-beta.2",     "1.0.0-beta.11"),     -1);
	EXPECT_EQ(sichCmpVersions("1.0.0-beta.11",    "1.0.0-rc.1"),        -1);
	EXPECT_EQ(sichCmpVersions("1.0.0-rc.1",       "1.0.0"),             -1);
}
TEST(SemanticVersioning, IgnoreBuildMetadata){
	EXPECT_EQ(sichCmpVersions("1.0.0-alpha+abc",       "1.0.0-alpha.1+def"),      -1);
	EXPECT_EQ(sichCmpVersions("1.0.0-alpha.1+abc",     "1.0.0-alpha.beta+abc0"),  -1);
	EXPECT_EQ(sichCmpVersions("1.0.0-alpha.beta+def",  "1.0.0-beta+abck"),        -1);
	EXPECT_EQ(sichCmpVersions("1.0.0-beta+def7",       "1.0.0-beta.2+def6"),      -1);
	EXPECT_EQ(sichCmpVersions("1.0.0-beta.2+abc1",     "1.0.0-beta.11+def0"),     -1);
	EXPECT_EQ(sichCmpVersions("1.0.0-beta.11+abc9",    "1.0.0-rc.1+abc8"),        -1);
	EXPECT_EQ(sichCmpVersions("1.0.0-rc.1+def0",       "1.0.0+abc1"),             -1);
	
	EXPECT_EQ(sichCmpVersions("1.0.0-alpha+abc",       "1.0.0-alpha+def"),         0);
	EXPECT_EQ(sichCmpVersions("1.0.0-alpha+def",       "1.0.0-alpha+abc"),         0);
}


TEST(PRNG, Lcg32){
	SICH_LCG32 S_STACK, *S = &S_STACK;
	sichLcg32Init(S, 0ull);
	
	ASSERT_EQ(sichLcg32Peek(S),                    0ull);
	ASSERT_EQ(sichLcg32Next(S),           2891336453ull);
	ASSERT_EQ(sichLcg32Next(S),           1192405134ull);
	ASSERT_EQ(sichLcg32Next(S),            568162667ull);
	ASSERT_EQ(sichLcg32Next(S),            878960812ull);
	ASSERT_EQ(sichLcg32Next(S),           3598456993ull);
	ASSERT_EQ(sichLcg32Next(S),           3109587930ull);
	ASSERT_EQ(sichLcg32Next(S),            457671207ull);
	ASSERT_EQ(sichLcg32Next(S),            564359576ull);
	ASSERT_EQ(sichLcg32Next(S),           1466123133ull);
	ASSERT_EQ(sichLcg32Next(S),           2469272934ull);
	ASSERT_EQ(sichLcg32Next(S),           2009562659ull);
}
TEST(PRNG, Lcg64){
	SICH_LCG64 S_STACK, *S = &S_STACK;
	sichLcg64Init(S, 0ull);
	
	ASSERT_EQ(sichLcg64Peek(S),                    0ull);
	ASSERT_EQ(sichLcg64Next(S),  1442695040888963407ull);
	ASSERT_EQ(sichLcg64Next(S),  1876011003808476466ull);
	ASSERT_EQ(sichLcg64Next(S), 11166244414315200793ull);
	ASSERT_EQ(sichLcg64Next(S),  7401132627792533940ull);
	ASSERT_EQ(sichLcg64Next(S),  7076646890315895283ull);
	ASSERT_EQ(sichLcg64Next(S), 10346034117385188870ull);
	ASSERT_EQ(sichLcg64Next(S),  1459328389850446429ull);
	ASSERT_EQ(sichLcg64Next(S),  6566661184467396264ull);
	ASSERT_EQ(sichLcg64Next(S),  8905969151499887831ull);
	ASSERT_EQ(sichLcg64Next(S),  8237903092696572954ull);
	ASSERT_EQ(sichLcg64Next(S), 11682375498643865569ull);
}
TEST(PRNG, Pcg32RxsMXs){
	SICH_LCG32 S_STACK, *S = &S_STACK;
	sichLcg32Init(S, 0ull);
	
	ASSERT_EQ(sichPcg32RxsMXsPeek(S),                    0ull);
	ASSERT_EQ(sichPcg32RxsMXsNext(S),            129708002ull);
	ASSERT_EQ(sichPcg32RxsMXsNext(S),            582399676ull);
	ASSERT_EQ(sichPcg32RxsMXsNext(S),           1006035121ull);
	ASSERT_EQ(sichPcg32RxsMXsNext(S),           1462727737ull);
	ASSERT_EQ(sichPcg32RxsMXsNext(S),           2264681948ull);
	ASSERT_EQ(sichPcg32RxsMXsNext(S),            879071586ull);
	ASSERT_EQ(sichPcg32RxsMXsNext(S),           1752778343ull);
	ASSERT_EQ(sichPcg32RxsMXsNext(S),           3722953945ull);
	ASSERT_EQ(sichPcg32RxsMXsNext(S),           1752290781ull);
	ASSERT_EQ(sichPcg32RxsMXsNext(S),           2484781302ull);
	ASSERT_EQ(sichPcg32RxsMXsNext(S),           2422741165ull);
}
TEST(PRNG, Pcg64XshRs){
	SICH_LCG64 S_STACK, *S = &S_STACK;
	sichLcg64Init(S, 0ull);
	
	ASSERT_EQ(sichPcg64XshRsPeek(S),                    0ull);
	ASSERT_EQ(sichPcg64XshRsNext(S),            367836042ull);
	ASSERT_EQ(sichPcg64XshRsNext(S),            599385756ull);
	ASSERT_EQ(sichPcg64XshRsNext(S),           3181286013ull);
	ASSERT_EQ(sichPcg64XshRsNext(S),           1527626195ull);
	ASSERT_EQ(sichPcg64XshRsNext(S),            447129947ull);
	ASSERT_EQ(sichPcg64XshRsNext(S),           3844031008ull);
	ASSERT_EQ(sichPcg64XshRsNext(S),             38569271ull);
	ASSERT_EQ(sichPcg64XshRsNext(S),            561439417ull);
	ASSERT_EQ(sichPcg64XshRsNext(S),           3425507267ull);
	ASSERT_EQ(sichPcg64XshRsNext(S),            695500562ull);
	ASSERT_EQ(sichPcg64XshRsNext(S),           1141113544ull);
}
TEST(PRNG, SkipAhead){
	SICH_LCG32 S32_STACK, *S32 = &S32_STACK;
	SICH_LCG64 S64_STACK, *S64 = &S64_STACK;
	sichLcg32Init(S32, 0ull);
	sichLcg64Init(S64, 0ull);
	
	ASSERT_EQ(sichLcg32Peek(S32),                    0ull);
	sichLcg32Skip(S32, -1);
	ASSERT_EQ(sichLcg32Next(S32),                    0ull);
	sichLcg32Skip(S32, 11);
	ASSERT_EQ(sichLcg32Peek(S32),           2009562659ull);
	sichLcg32Skip(S32, -11);
	ASSERT_EQ(sichLcg32Peek(S32),                    0ull);
	
	
	ASSERT_EQ(sichLcg64Peek(S64),                    0ull);
	sichLcg64Skip(S64, -1);
	ASSERT_EQ(sichLcg64Next(S64),                    0ull);
	sichLcg64Skip(S64, 11);
	ASSERT_EQ(sichLcg64Peek(S64), 11682375498643865569ull);
	sichLcg64Skip(S64, -11);
	ASSERT_EQ(sichLcg64Peek(S64),                    0ull);
}
TEST(PRNG, Dist){
	SICH_LCG32 S32_STACK, *S32 = &S32_STACK;
	SICH_LCG64 S64_STACK, *S64 = &S64_STACK;
	sichLcg32Init(S32, 0ull);
	sichLcg64Init(S64, 0ull);
	
	sichLcg32Skip(S32, 0xDEADBEEF);
	ASSERT_EQ(sichLcg32Dist(S32),           0xDEADBEEFull);
	sichLcg32Skip(S32, -46);
	ASSERT_EQ(sichLcg32Dist(S32),           0xDEADBEEFull-46);
	
	sichLcg64Skip(S64, 0xDEADBEEFDEADBEEF);
	ASSERT_EQ(sichLcg64Dist(S64),   0xDEADBEEFDEADBEEFull);
	sichLcg64Skip(S64, -46);
	ASSERT_EQ(sichLcg64Dist(S64),   0xDEADBEEFDEADBEEFull-46);
}
TEST(PRNG, Diff){
	SICH_LCG32 Ss32_STACK, *Ss32 = &Ss32_STACK, Se32_STACK, *Se32 = &Se32_STACK;
	SICH_LCG64 Ss64_STACK, *Ss64 = &Ss64_STACK, Se64_STACK, *Se64 = &Se64_STACK;
	sichLcg32Init(Ss32, 0ull);
	sichLcg32Init(Se32, 0ull);
	sichLcg64Init(Ss64, 0ull);
	sichLcg64Init(Se64, 0ull);
	
	sichLcg32Skip(Ss32, 0xDEADBEEF);
	sichLcg32Skip(Se32, 0xDEADBEEF+0xCAFECAFE);
	sichLcg64Skip(Ss64, 0xDEADBEEFDEADBEEF);
	sichLcg64Skip(Se64, 0xDEADBEEFDEADBEEF+0xCAFECAFECAFECAFE);
	
	ASSERT_EQ(sichLcg32Diff(Ss32, Se32),           0xCAFECAFE);
	ASSERT_EQ(sichLcg32Diff(Se32, Ss32),          -0xCAFECAFE);
	ASSERT_EQ(sichLcg64Diff(Ss64, Se64),   0xCAFECAFECAFECAFE);
	ASSERT_EQ(sichLcg64Diff(Se64, Ss64),  -0xCAFECAFECAFECAFE);
}

