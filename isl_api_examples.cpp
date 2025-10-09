#include "isl_api_examples.h"

#include <iostream>
#include <string>

#include "include/isl/ast.h"
#include "include/isl/ast_build.h"
#include "include/isl/ast_type.h"
#include "include/isl/ctx.h"
#include "include/isl/map.h"
#include "include/isl/map_type.h"
#include "include/isl/point.h"
#include "include/isl/set.h"
#include "include/isl/space.h"
#include "include/isl/space_type.h"
#include "include/isl/union_map.h"
#include "include/isl/val.h"
#include "include/isl/val_type.h"

std::string parse_map_and_extract_domain_as_string(isl_ctx *ctx,
                                                   std::string isl_str) {
  isl_set *domainMap =
      isl_map_domain(isl_map_read_from_str(ctx, isl_str.c_str()));
  char *result_str = isl_set_to_str(domainMap);
  std::string result(result_str);

  free(result_str);
  isl_set_free(domainMap);
  return result;
}

// Allocates an isl_mat with the correct size for a given space.
__isl_give isl_mat *create_empty_constraint_matrix(__isl_keep isl_space *space,
                                                   unsigned num_constraints) {
  unsigned num_in = isl_space_dim(space, isl_dim_in);
  unsigned num_out = isl_space_dim(space, isl_dim_out);
  unsigned num_div = isl_space_dim(space, isl_dim_div);
  unsigned num_param = isl_space_dim(space, isl_dim_param);

  // The layout of the columns is: [domain_vars, range_vars, div_vars,
  // symbol_vars, constant]
  unsigned num_cols = num_in + num_out + num_div + num_param + 1;

  isl_mat *mat =
      isl_mat_alloc(isl_space_get_ctx(space), num_constraints, num_cols);

  // Initialize to zero (needed?)
  for (int i = 0; i < isl_mat_rows(mat); ++i)
    for (int j = 0; j < isl_mat_cols(mat); ++j)
      isl_mat_set_element_si(mat, /*row=*/i, /*col=*/j, 0);
  return mat;
}

std::string precompose_transposition(isl_ctx *ctx, std::string starting_layout,
                                     int dim1, int dim2) {
  isl_map *layout = isl_map_read_from_str(ctx, starting_layout.c_str());
  isl_set *domain = isl_map_domain(isl_map_copy(layout));

  isl_map *domain_map =
      isl_map_from_domain_and_range(isl_set_copy(domain), domain);
  isl_space *transpose_space = isl_map_get_space(domain_map);
  isl_map_free(domain_map);
  unsigned num_domain_vars = isl_space_dim(transpose_space, isl_dim_in);

  isl_mat *eq_mat =
      create_empty_constraint_matrix(transpose_space, num_domain_vars);
  isl_mat *ineq_mat = create_empty_constraint_matrix(transpose_space, 0);

  // Column order is: [domain_vars, range_vars, div_vars, symbol_vars, constant]
  // so the offset between a domain variable and its corresponding range
  // variable is num_domain_vars
  for (int domain_var = 0; domain_var < num_domain_vars; ++domain_var) {
    // First constraint: domain_var dim1 - range_var dim2 = 0
    if (domain_var == dim1) {
      isl_mat_set_element_si(eq_mat, /*row=*/dim1, /*col=*/dim1, 1);
      isl_mat_set_element_si(eq_mat, /*row=*/dim1,
                             /*col=*/num_domain_vars + dim2, -1);
      continue;
    }

    if (domain_var == dim2) {
      // Second constraint: domain_var dim2 - range_var dim1 = 0
      isl_mat_set_element_si(eq_mat, /*row=*/dim2, /*col=*/dim2, 1);
      isl_mat_set_element_si(eq_mat, /*row=*/dim2,
                             /*col=*/num_domain_vars + dim1, -1);
      continue;
    }

    // Otherwise, domain_var d - range_var d = 0
    isl_mat_set_element_si(eq_mat, /*row=*/domain_var, /*col=*/domain_var, 1);
    isl_mat_set_element_si(eq_mat, /*row=*/domain_var,
                           /*col=*/num_domain_vars + domain_var, -1);
  }

  isl_map *transpose =
      isl_map_from_basic_map(isl_basic_map_from_constraint_matrices(
          transpose_space, eq_mat, ineq_mat, isl_dim_in, isl_dim_out,
          isl_dim_div, isl_dim_param, isl_dim_cst));

  isl_map *composed = isl_map_apply_range(transpose, layout);

  char *result_str = isl_map_to_str(composed);
  std::string result(result_str);

  free(result_str);
  isl_map_free(composed);
  return result;
}

void print_matrices(isl_ctx *ctx, std::string isl_str) {
  isl_basic_set *set = isl_basic_set_read_from_str(ctx, isl_str.c_str());

  isl_mat *eq_mat = isl_basic_set_equalities_matrix(
      set, isl_dim_set, isl_dim_div, isl_dim_param, isl_dim_cst);

  std::cout << "Equality Matrix:\n" << std::endl;
  if (isl_mat_rows(eq_mat) == 0) {
    std::cout << "(No equalities)" << std::endl;
  } else {
    isl_mat_dump(eq_mat);
    std::cout << std::endl;
  }

  std::cout << "\n" << std::endl;

  isl_mat *ineq_mat = isl_basic_set_inequalities_matrix(
      set, isl_dim_set, isl_dim_param, isl_dim_div, isl_dim_cst);

  std::cout << "Inequality Matrix:\n" << std::endl;
  if (isl_mat_rows(ineq_mat) == 0) {
    std::cout << "(No inequalities)" << std::endl;
  } else {
    isl_mat_dump(ineq_mat);
    std::cout << std::endl;
  }

  isl_mat_free(eq_mat);
  isl_mat_free(ineq_mat);
  isl_basic_set_free(set);
}

isl_stat pointPairCallback(__isl_take isl_point *pnt, void *user) {
  PointPairCollector *collector = static_cast<PointPairCollector *>(user);

  isl_space *space = isl_point_get_space(pnt);
  isl_space_free(space);

  std::vector<int64_t> domainPoint(collector->domain_dims);
  std::vector<int64_t> rangePoint(collector->range_dims);

  // Extract domain coordinates
  for (int i = 0; i < collector->domain_dims; i++) {
    isl_val *coord = isl_point_get_coordinate_val(pnt, isl_dim_set, i);
    if (isl_val_is_int(coord)) {
      domainPoint[i] = isl_val_get_num_si(coord);
    }
    isl_val_free(coord);
  }

  // Extract range coordinates
  for (int i = 0; i < collector->range_dims; i++) {
    isl_val *coord = isl_point_get_coordinate_val(pnt, isl_dim_set,
                                                  collector->domain_dims + i);
    if (isl_val_is_int(coord)) {
      rangePoint[i] = isl_val_get_num_si(coord);
    }
    isl_val_free(coord);
  }

  collector->points.emplace_back(domainPoint, rangePoint);
  isl_point_free(pnt);
  return isl_stat_ok;
}

void enumerate_points(isl_basic_map *bmap, PointPairCollector &collector) {
  isl_set *set = isl_set_from_basic_set(isl_basic_map_wrap(bmap));
  isl_set_foreach_point(set, &pointPairCallback, &collector);
  isl_set_free(set);
}

std::string generate_loop_nest_as_c_str(isl_basic_map *bmap) {
  auto *ctx = isl_basic_map_get_ctx(bmap);
  isl_union_map *schedule =
      isl_union_map_from_basic_map(isl_basic_map_copy(bmap));

  // Context and options are intentionally empty.
  isl_set *context = isl_set_universe(isl_space_params_alloc(ctx, 0));
  isl_union_map *options = isl_union_map_empty(isl_space_params_alloc(ctx, 0));

  // Build the AST
  isl_ast_build *build = isl_ast_build_from_context(context);
  build = isl_ast_build_set_options(build, options);
  isl_ast_node *tree = isl_ast_build_node_from_schedule_map(build, schedule);
  isl_ast_build_free(build);

  char *cStr = isl_ast_node_to_C_str(tree);
  std::string actual = std::string(cStr);
  free(cStr);
  isl_ast_node_free(tree);
  isl_ctx_free(ctx);

  // Add a leading newline for ease of comparison with multiline strings.
  return actual.insert(0, "\n");
}
