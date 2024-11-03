#pragma once

#include <heuristics.hpp>

typedef struct Args {
  char* in_filename;
  char* out_filename;
  double tolerance;
  bool mode; // 0 - encode, 1 - decode
  heuristic::mh_t match_heur;
  heuristic::idh_t dict_init_heur;
  heuristic::gpuh_t growing_point_update_heur;
  heuristic::gh_t growing_heur;
  heuristic::duh_t dict_update_heur;
  heuristic::dh_t deletion_heur;
  bool exit;
} Args;