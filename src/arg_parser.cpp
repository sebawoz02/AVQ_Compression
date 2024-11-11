#include <arg_parser.hpp>
#include <iostream>
#include <string>

namespace arg_parser {
  static void set_default_values(Args& args, char** argv)
  {
    args.in_filename = argv[1];
    args.out_filename = argv[2];
    args.tolerance = 0.0;
    args.match_heur = heuristic::match::top_left_max_se;
    args.dict_init_heur = heuristic::dict_init::range_0_to_255;
    args.growing_point_update_heur = heuristic::gpp_update::first_from_left;
    args.growing_heur = heuristic::growing::wave;
    args.dict_update_heur = heuristic::dict_update::one_column_one_row;
    args.deletion_heur = heuristic::dict_deletion::fifo;
  }

  Args parse(size_t argc, char** argv)
  {
    Args args{};
    args.exit = true;
    if(argc < 2) {
      std::cout
        << "Usage: ./avq <input_file> <output_file> [-e/-d] [<options>] \n"
           "./avq --help for more\n";
      return args;
    }
    std::string tmp = argv[1];
    if(tmp == "--help" || tmp == "-h") {
      std::cout
        << "Usage: ./avq <input_file> <output_file> [-e/-d] [<options>] \n";
      std::cout << "--help         Display this help message\n";
      std::cout << "-e             Encode mode\n";
      std::cout << "-d             Decode mode\n";
      std::cout
        << "-ac            Additional adaptive arithmetic compression\n";
      std::cout << "-t <value>     Match tolerance [0.0, 1.0] - default: 0.0\n"
                   "               0 - no tolerance for information loss in "
                   "compression\n";
      std::cout << "-mh <value>    Match heuristic:\n"
                   "               0 - Maximum Square Error (default)\n"
                   "               1 - Mean Square Error\n";
      std::cout << "-gh <value>    Growing heuristic:\n"
                   "               0 - Wave (default)\n"
                   "               1 - Diagonal\n"
                   "               2 - LIFO\n";
      std::cout << "-duh <value>   Dictionary update heuristic:\n"
                   "               0 - One Column One Row (default)\n"
                   "               1 - One Row\n"
                   "               2 - One Column\n";
      return args;
    }

    if(argc < 4) {
      std::cout
        << "Usage: ./avq <input_file> <output_file> [-e/-d] [<options>] \n";
      return args;
    }

    // Default values
    set_default_values(args, argv);

    bool mode = false;
    for(size_t i = 3; i < argc; i++) {
      std::string arg = argv[i];
      if(arg == "-e") {
        if(mode) {
          std::cerr << "Use -e/-d flag only once" << std::endl;
          return args;
        }
        mode = true;
        args.mode = false;
      } else if(arg == "-d") {
        if(mode) {
          std::cerr << "Use -e/-d flag only once" << std::endl;
          return args;
        }
        mode = true;
        args.mode = true;
      } else if(arg == "-t") {
        double tolerance;
        try {
          tolerance = std::stod(argv[i + 1]);
        } catch(const std::invalid_argument& e) {
          std::cerr << "Error: Invalid number format for tolerance."
                    << std::endl;
          return args;
        } catch(const std::out_of_range& e) {
          std::cerr << "Error: Tolerance value out of range." << std::endl;
          return args;
        }
        if(tolerance < 0.0 || tolerance > 1.0) {
          std::cerr << "Tolerance needs to be value in range [0,1]"
                    << std::endl;
          return args;
        }
        args.tolerance = tolerance;
        i++;
      } else if(arg == "-gh") {
        int m;
        try {
          m = std::stoi(argv[i + 1]);
        } catch(const std::invalid_argument& e) {
          std::cerr << "Error: Invalid number format for growing heuristic."
                    << std::endl;
          return args;
        } catch(const std::out_of_range& e) {
          std::cerr << "Error: Growing heuristic value out of range."
                    << std::endl;
          return args;
        }
        if(m == 1) {
          args.growing_heur = heuristic::growing::diagonal;
        } else if(m == 2) {
          args.growing_heur = heuristic::growing::lifo;
        }
        i++;
      } else if(arg == "-duh") {
        int m;
        try {
          m = std::stoi(argv[i + 1]);
        } catch(const std::invalid_argument& e) {
          std::cerr << "Error: Invalid number format for dict update heuristic."
                    << std::endl;
          return args;
        } catch(const std::out_of_range& e) {
          std::cerr << "Error: Dict update heuristic value out of range."
                    << std::endl;
          return args;
        }
        if(m == 1) {
          args.dict_update_heur = heuristic::dict_update::one_row;
        } else if(m == 2) {
          args.dict_update_heur = heuristic::dict_update::one_column;
        }
        i++;
      } else if(arg == "-mh") {
        int m;
        try {
          m = std::stoi(argv[i + 1]);
        } catch(const std::invalid_argument& e) {
          std::cerr << "Error: Invalid number format for match heuristic."
                    << std::endl;
          return args;
        } catch(const std::out_of_range& e) {
          std::cerr << "Error: Match heuristic value out of range."
                    << std::endl;
          return args;
        }
        if(m == 1) {
          args.match_heur = heuristic::match::top_left_mse;
        }
        i++;
      } else {
        std::cout << "Unknown option: " << arg << "\n";
        return args;
      }
    }
    if(!mode) {
      std::cerr << "Specify mode with -e/-d flag." << std::endl;
      return args;
    }

    args.exit = false;
    return args;
  }
} // namespace arg_parser
