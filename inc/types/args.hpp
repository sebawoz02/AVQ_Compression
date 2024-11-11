#pragma once

#include <heuristics.hpp>

/**
 * @struct Args
 * @brief Data provided when starting the program or their default values.
 *
 */
typedef struct Args {
  char* in_filename;                            ///< Input file
  char* out_filename;                           ///< Output file
  double tolerance;                             ///< Information loss tolerance range=<0.0, 1.0>
  bool mode;                                    ///< Coder mode: 0 - encode, 1 - decode
  heuristic::mh_t match_heur;                   ///< Block matching heuristic
  heuristic::idh_t dict_init_heur;              ///< Dictionary initialization heuristic
  heuristic::gpuh_t growing_point_update_heur;  ///< Growing Point Pool update heuristic
  heuristic::gh_t growing_heur;                 ///< Growing heuristic
  heuristic::duh_t dict_update_heur;            ///< Dictionary update heuristic
  heuristic::dh_t deletion_heur;                ///< Dictionary entry deletion heuristic
  bool exit;                                    ///< Error code
} Args;