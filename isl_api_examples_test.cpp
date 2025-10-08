#include "isl_api_examples.h"

#include <string>

#include "gtest/gtest.h"
#include "include/isl/ctx.h"

namespace {

TEST(IslApiExamplesTest, TestParseMapAndExtractDomainAsString) {
  std::string islMap =
      "{ [i] -> [j] : (-i + j) mod 7 = 0 and 0 <= i <= 20 and 0 <= j <= 20 }";
  std::string expected = "{ [i] : 0 <= i <= 20 }";

  isl_ctx* ctx = isl_ctx_alloc();
  std::string actual = parse_map_and_extract_domain_as_string(ctx, islMap);
  EXPECT_EQ(expected, actual);
  isl_ctx_free(ctx);
}

TEST(IslApiExamplesTest, TestParseMapAndExtractDomainAsStringSubset) {
  std::string islMap =
      "{ [i] -> [j] : (-i + j) mod 7 = 0 and 0 <= i <= 20 and 0 <= j <= 3 }";
  std::string expected =
      "{ [i] : 0 <= i <= 20 and 7*floor((-1 - i)/7) <= -4 - i }";

  isl_ctx* ctx = isl_ctx_alloc();
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
      "{ S[i, j, k] -> [reg, lane] : (-32i - j + 16reg + lane) mod 2048 = 0 "
      "and 0 <= i <= 63 and 0 <= j <= 31 and lane >= 32i + j - 16reg and 0 <= "
      "lane <= 15 and lane <= 2095104 + 32i + j - 16reg }";

  isl_ctx* ctx = isl_ctx_alloc();
  std::string actual = precompose_transposition(ctx, islMap, 0, 1);
  EXPECT_EQ(expected, actual);
  isl_ctx_free(ctx);
}

}  // namespace
