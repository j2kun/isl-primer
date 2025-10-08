// A binary that runs precompose_transposition from a stdin string

#include <iostream>
#include <string>

#include "include/isl/ctx.h"
#include "isl_api_examples.h"

int main() {
  std::string islMap;
  std::cout << "Enter an ISL map: ";
  std::getline(std::cin, islMap);

  isl_ctx* ctx = isl_ctx_alloc();
  std::string result = precompose_transposition(ctx, islMap, 0, 1);
  std::cout << "Result: " << result << std::endl;
  isl_ctx_free(ctx);
  return 0;
}
