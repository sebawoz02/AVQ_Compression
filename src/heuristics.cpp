#include <heuristics.hpp>
#include <cmath>

namespace heuristic {
    // MATCH
    double match::mse(Block *block1, Block *block2)
    {
        (void) block1;
        (void) block2;
        return (double)0.0;
    }

    // DICT INIT
    Dictionary* dict_init::range_0_to_255(std::vector<std::vector<uint8_t>>& pixels)
    {
        (void) pixels;
        auto* dict = new Dictionary();
        for (size_t color = 0; color < 256; color++)
        {
            std::vector<std::vector<uint8_t>> pixel(std::vector<std::vector<uint8_t>>(1, std::vector<uint8_t>(1, 0)));
            pixel[0][0] = color;
            auto* block = new Block(1, 1, pixel);
            dict->insert(block);
        }

        return dict;
    }

    // GP UPDATE
    void gp_update::first_from_left(std::vector<Growing_point*>& growing_points, size_t* size, Growing_point* cur_gp)
    {
        (void) growing_points;
        (void) size;
        (void) cur_gp;
        // TODO: how to add gp, how to choose its size
        // concept:
        // niech struct GP sklada sie jedynie z x, y.
        // nastepnie wybierane są rozne mozliwe dopasowania podobne kształtem do wejść w dictionary
        // minusy:
        // - kosztowna operacja
    }

    // GROWING
    Growing_point* growing::wave(std::vector<Growing_point*>& gp_pool)
    {
        Growing_point* best = gp_pool[0];
        for (size_t i = 1; i < gp_pool.size(); i++)
        {
            Growing_point* cur = gp_pool[i];
            if (best->x + best->y > cur->x + cur->y)
            {
                best = cur;
            }
        }

        return best;
    }

    Growing_point* growing::diagonal(std::vector<Growing_point*>& gp_pool)
    {
        Growing_point* best = gp_pool[0];
        for (size_t i = 1; i < gp_pool.size(); i++)
        {
            Growing_point* cur = gp_pool[i];
            if (abs((int)best->x - (int)best->y) > abs((int)cur->x + (int)cur->y))
            {
                best = cur;
            }
        }

        return best;
    }

    Growing_point* growing::lifo(std::vector<Growing_point*>& gp_pool)
    {
        return gp_pool[gp_pool.size() - 1];
    }
}
