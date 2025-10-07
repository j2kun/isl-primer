#include "isl_api_examples.h"

#include <string>

#include "include/isl/ctx.h"
#include "include/isl/map.h"
#include "include/isl/map_type.h"
#include "include/isl/set.h"

std::string parse_map_and_extract_domain_as_string(isl_ctx *ctx,
                                                   std::string islStr) {
  isl_set *domainMap =
      isl_map_domain(isl_map_read_from_str(ctx, islStr.c_str()));
  char *resultStr = isl_set_to_str(domainMap);
  std::string result(resultStr);

  free(resultStr);
  isl_set_free(domainMap);
  return result;
}
