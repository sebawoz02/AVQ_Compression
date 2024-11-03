#pragma once

#include <types/args.hpp>

namespace arg_parser {
  Args parse(size_t argc, char** argv);
}