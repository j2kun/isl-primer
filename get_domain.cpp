// A binary that runs parse_map_and_extract_domain_as_string from a stdin string

#include <iostream>
#include <string>

#include "isl_api_examples.h"
#include "include/isl/ctx.h"

int main() {
  std::string islMap;
  std::cout << "Enter an ISL map: ";
  std::getline(std::cin, islMap);

  isl_ctx *ctx = isl_ctx_alloc();
  std::string result = parse_map_and_extract_domain_as_string(ctx, islMap);
  std::cout << "Result: " << result << std::endl;
  isl_ctx_free(ctx);
  return 0;
}
