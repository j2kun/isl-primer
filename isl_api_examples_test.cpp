#include "isl_api_examples.h"

#include <string>

#include "gtest/gtest.h"
#include "include/isl/ctx.h"

namespace {

TEST(IslApiExamplesTest, TestParseMapAndExtractDomainAsString) {
  std::string islMap =
      "{ [i] -> [j] : (-i + j) mod 7 = 0 and 0 <= i <= 20 and 0 <= j <= 20 }";
  std::string expected =
      "{ [i] : 0 <= i <= 20 }";

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

} // namespace
