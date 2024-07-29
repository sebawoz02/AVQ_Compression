#pragma once

#include <dictionary.hpp>
#include <growing_point.hpp>

namespace heuristic {
    using mh_t = double (*)(Block*, Block*);

    namespace match {
        double mse(Block* block1, Block* block2);
    }

    using idh_t = Dictionary* (*)(Block*);

    using gpuh_t = void (*)(std::vector<Growing_point*>& growing_points);

    using gh_t = Growing_point* (*)(std::vector<Growing_point*>& growing_points);

    using duh_t = void (*)(Dictionary* dict, Block* image);

    using dh_t = void (*)(Dictionary* dict);
}