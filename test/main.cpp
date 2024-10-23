#include <test_common.hpp>

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    test_dictionary();
    test_gp_pool();
    test_heuristics();

    return 0;
}