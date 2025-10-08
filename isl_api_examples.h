#include <string>

#include "include/isl/ctx.h"

std::string parse_map_and_extract_domain_as_string(isl_ctx* ctx,
                                                   std::string islStr);

// Precompose with startingLayout a layout that transposes two domain
// dimensions, and return the resulting layout as a string
std::string precompose_transposition(isl_ctx* ctx, std::string startingLayout,
                                     int dim1, int dim2);
