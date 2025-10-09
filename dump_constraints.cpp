#include <iostream>
#include <string>

#include "isl_api_examples.h"
#include "include/isl/ctx.h"

int main() {
  std::string isl_map;
  std::cout << "Enter an ISL map: ";
  std::getline(std::cin, isl_map);

  isl_ctx *ctx = isl_ctx_alloc();
  print_matrices(ctx, isl_map);
  isl_ctx_free(ctx);
  return 0;
}
