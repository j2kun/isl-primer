#include <iostream>
#include <string>

#include "include/isl/ctx.h"
#include "include/isl/map.h"
#include "isl_api_examples.h"

int main() {
  std::string isl_map;
  std::cout << "Enter an ISL map: ";
  std::getline(std::cin, isl_map);

  isl_ctx *ctx = isl_ctx_alloc();
  isl_basic_map *bmap = isl_basic_map_read_from_str(ctx, isl_map.c_str());
  PointPairCollector collector(isl_basic_map_dim(bmap, isl_dim_in),
                               isl_basic_map_dim(bmap, isl_dim_out));
  enumerate_points(bmap, collector);

  for (const auto& p : collector.points) {
    std::cout << "[";
    for (size_t i = 0; i < p.first.size(); ++i) {
      std::cout << p.first[i];
      if (i + 1 < p.first.size()) std::cout << ", ";
    }
    std::cout << "] -> [";
    for (size_t i = 0; i < p.second.size(); ++i) {
      std::cout << p.second[i];
      if (i + 1 < p.second.size()) std::cout << ", ";
    }
    std::cout << "]\n";
  }

  isl_ctx_free(ctx);
  return 0;
}
