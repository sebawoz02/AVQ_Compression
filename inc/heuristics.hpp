#pragma once

#include <dictionary.hpp>
#include <growing_point.hpp>

namespace heuristic {
    using mh_t = double (*)(Block*, Block*);

    using idh_t = Dictionary* (*)();

    using gpuh_t = void (*)(std::vector<Growing_point*>&, size_t*, Growing_point*);

    using gh_t = Growing_point* (*)(std::vector<Growing_point*>&);

    using duh_t = void (*)(Dictionary*, Growing_point*, std::vector<std::vector<uint8_t>>&);

    using dh_t = void (*)(Dictionary*);

    namespace match {
        double mse(Block* block1, Block* block2);
    }

    namespace dict_init {
        Dictionary* range_0_to_255();
    }

    namespace gp_update {
        void first_from_left(std::vector<Growing_point*>& growing_points, size_t* size, Growing_point* cur_gp);
    }

    namespace growing {
        Growing_point* wave(std::vector<Growing_point*>& gp_pool);
        Growing_point* diagonal(std::vector<Growing_point*>& gp_pool);
        Growing_point* lifo(std::vector<Growing_point*>& gp_pool);
    }

    namespace dict_update {
        void one_column(Dictionary* dict, Growing_point* gp, std::vector<std::vector<uint8_t>>& image);
        void one_row(Dictionary *dict, Growing_point *gp, std::vector<std::vector<uint8_t>> &image);
        void one_column_one_row(Dictionary *dict, Growing_point *gp, std::vector<std::vector<uint8_t>> &image);
    }

    namespace dict_deletion {
        void fifo(Dictionary* dict);
    }
}