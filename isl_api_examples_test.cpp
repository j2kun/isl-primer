#include "isl_api_examples.h"

#include <string>

#include "include/isl/ctx.h"
#include "gtest/gtest.h"

namespace {

TEST(IslApiExamplesTest, TestParseMapAndExtractDomainAsString) {
  std::string islMap =
      "{ [i] -> [j] : (-i + j) mod 7 = 0 and 0 <= i <= 20 and 0 <= j <= 20 }";
  std::string expected = "{ [i] : 0 <= i <= 20 }";

  isl_ctx *ctx = isl_ctx_alloc();
  std::string actual = parse_map_and_extract_domain_as_string(ctx, islMap);
  EXPECT_EQ(expected, actual);
  isl_ctx_free(ctx);
}

TEST(IslApiExamplesTest, TestParseMapAndExtractDomainAsStringSubset) {
  std::string islMap =
      "{ [i] -> [j] : (-i + j) mod 7 = 0 and 0 <= i <= 20 and 0 <= j <= 3 }";
  std::string expected =
      "{ [i] : 0 <= i <= 20 and 7*floor((-1 - i)/7) <= -4 - i }";

  isl_ctx *ctx = isl_ctx_alloc();
  std::string actual = parse_map_and_extract_domain_as_string(ctx, islMap);
  EXPECT_EQ(expected, actual);
  isl_ctx_free(ctx);
}

TEST(IslApiExamplesTest, TestPrecomposeTransposition) {
  std::string islMap =
      "{ S[i, j, k] -> [reg, lane] : 0 <= i < 32 and 0 <= j < 64 and 0 <= k < "
      "1024 and 0 <= lane < 16 and i + j * 32 + k * 32 * 64 = 16 * reg + lane "
      "}";
  std::string expected =
      "{ S[i, j, k] -> [reg, lane = 32i + j + 2048k - 16reg] : 0 <= i <= 63 "
      "and 0 <= j <= 31 and 0 <= k <= 1023 and -15 + 32i + j + 2048k <= 16reg "
      "<= 32i + j + 2048k }";

  isl_ctx *ctx = isl_ctx_alloc();
  std::string actual = precompose_transposition(ctx, islMap, 0, 1);
  EXPECT_EQ(expected, actual);
  isl_ctx_free(ctx);
}

TEST(IslApiExamplesTest, TestEnumeratePoints) {
  // Create a relation with 1 domain variable (x) and 1 range variable (y)
  isl_ctx *ctx = isl_ctx_alloc();
  isl_basic_map *bmap = isl_basic_map_read_from_str(
      ctx, "{ [x] -> [y] : x >= 0 and 2 >= x and y >= 0 and 1 >= y }");
  PointPairCollector collector(1, 1); // 1 domain dim, 1 range dim
  enumerate_points(bmap, collector);

  // Expected points: domain x range pairs for x in [0,2] and y in [0,1]
  std::vector<std::pair<std::vector<int64_t>, std::vector<int64_t>>> expected =
      {{{0}, {0}}, {{0}, {1}}, {{1}, {0}}, {{1}, {1}}, {{2}, {0}}, {{2}, {1}}};

  EXPECT_EQ(collector.points.size(), expected.size());
  for (const auto &expectedPoint : expected) {
    bool found = false;
    for (const auto &actualPoint : collector.points) {
      if (actualPoint.first == expectedPoint.first &&
          actualPoint.second == expectedPoint.second) {
        found = true;
        break;
      }
    }
    EXPECT_TRUE(found) << "Expected point not found: domain="
                       << expectedPoint.first[0]
                       << ", range=" << expectedPoint.second[0];
  }

  isl_ctx_free(ctx);
}

TEST(IslApiExamplesTest, TestGenerateLoopNestAsCStr) {
  isl_ctx *ctx = isl_ctx_alloc();
  auto *bmap = isl_basic_map_read_from_str(
      ctx,
      "{ S[d0, d1] -> [d1] : d0 - d1 = 0 and 0 <= d0 <= 10 and 0 <= d1 <= 10 }");
  auto actual = generate_loop_nest_as_c_str(bmap);
  std::string expected = R"(
for (int c0 = 0; c0 <= 10; c0 += 1)
  S(c0, c0);
)";
  EXPECT_EQ(actual, expected);

  isl_ctx_free(ctx);
  isl_basic_map_free(bmap);
}

} // namespace
