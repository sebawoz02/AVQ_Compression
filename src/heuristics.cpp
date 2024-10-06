#include <heuristics.hpp>
#include <cmath>


#define DICT_SIZE_LIMIT 512

namespace heuristic {
    // MATCH
    double match::mse(Block *block1, Block *block2)
    {
        (void) block1;
        (void) block2;
        return (double)0.0;
    }

    // DICT INIT
    Dictionary* dict_init::range_0_to_255()
    {
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

    // DICT UPDATE
    void dict_update::one_column(Dictionary *dict, Growing_point *gp, std::vector<std::vector<uint8_t>> &image)
    {
        if (gp->y == 0)
        {
            auto* b = new Block(gp->block->width, gp->block->height, gp->block->pixels);
            dict->insert(b);
            return;
        }
        size_t w = gp->block->width;
        size_t h = gp->block->height + 1;

        std::vector<std::vector<uint8_t>> pixels(std::vector<std::vector<uint8_t>>(w, std::vector<uint8_t>(h, 0)));
        for (size_t x = 0; x < w ; x++)
        {
            pixels[x][0] = image[gp->x + x][gp->y - 1];
            for (size_t y = 1; y < h; y++)
            {
                pixels[x][y] = gp->block->pixels[x][y - 1];
            }
        }
        auto* b = new Block(w, h, pixels);
        dict->insert(b);
    }

    void dict_update::one_row(Dictionary *dict, Growing_point *gp, std::vector<std::vector<uint8_t>> &image)
    {
        if (gp->x == 0)
        {
            auto* b = new Block(gp->block->width, gp->block->height, gp->block->pixels);
            dict->insert(b);
            return;
        }
        size_t w = gp->block->width + 1;
        size_t h = gp->block->height;

        std::vector<std::vector<uint8_t>> pixels(std::vector<std::vector<uint8_t>>(w, std::vector<uint8_t>(h, 0)));
        for (size_t y = 0; y < w ; y++)
        {
            pixels[0][y] = image[gp->x - 1][gp->y + y];
            for (size_t x = 1; x < h; x++)
            {
                pixels[x][y] = gp->block->pixels[x - 1][y];
            }
        }
        auto* b = new Block(w, h, pixels);
        dict->insert(b);
    }

    void dict_update::one_column_one_row(Dictionary *dict, Growing_point *gp, std::vector<std::vector<uint8_t>> &image)
    {
        one_column(dict, gp, image);
        one_row(dict, gp, image);
    }

    // DICT DELETION
    void dict_deletion::fifo(Dictionary* dict)
    {
        // TODO: FIX THIS logic
        while(dict->size > DICT_SIZE_LIMIT)
        {
            delete dict->entries[256];  // DON'T TOUCH 1x1 blocks
            dict->entries.erase(dict->entries.begin() + 256);
            dict->size--;
        }
    }
}
