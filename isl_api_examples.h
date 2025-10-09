#include <string>
#include <vector>

#include "include/isl/ctx.h"
#include "include/isl/map.h"

std::string parse_map_and_extract_domain_as_string(isl_ctx* ctx,
                                                   std::string isl_str);

// Precompose with startingLayout a layout that transposes two domain
// dimensions, and return the resulting layout as a string
std::string precompose_transposition(isl_ctx* ctx, std::string startingLayout,
                                     int dim1, int dim2);

// Parse and print the constraint matrices
void print_matrices(isl_ctx* ctx, std::string isl_str);


// A struct for use in collecting domain and range points during enumeration
struct PointPairCollector {
  using Point = std::vector<int64_t>;
  std::vector<std::pair<Point, Point>> points;
  int domain_dims;
  int range_dims;

  PointPairCollector(int domain_dims, int range_dims)
      : domain_dims(domain_dims), range_dims(range_dims) {}
};

void enumerate_points(isl_basic_map *bmap, PointPairCollector& collector);

std::string generate_loop_nest_as_c_str(isl_basic_map* bmap);
