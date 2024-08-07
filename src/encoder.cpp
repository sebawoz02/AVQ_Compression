#include <encoder.hpp>
#include <growing_point.hpp>
#include <cmath>

namespace encoder {

    void Encoder::encode(const Image& image)
    {
        // Run AVQ for each color
    }

    void Encoder::adaptive_vector_quantization(ColorMap* const image)
    {
        // Add pixel [0, 0] to growing points pool
        auto* init_gp = new Growing_point(0, 0, 1, 1, image);
        std::vector<Growing_point*> gp_pool{};
        gp_pool.push_back(init_gp);

        size_t gpp_size = 1;

        // Init dictionary
        Dictionary* dict = dict_init_heur(image);

        // While growing points pool has more elements
        while (gpp_size > 0)
        {
            // Determine next growing point
            Growing_point* gp = growing_heur(gp_pool);
            gpp_size--; // Current gp removed from growing points pool

            // TODO: Use MH to find a block 'common_block' in D that matches the sub-block anchored to gp

            size_t common_block_idx = 0;

            auto bits_to_transmit = static_cast<size_t>(
                    std::floor(log2(static_cast<double>(dict->size))));

            // Write common_block_idx to file on log2(dict.size) bits
            for(uint8_t i = bits_to_transmit - 1; i >= 0; i--)
            {
                bool bit = (common_block_idx >> i) & 1;
                // TODO: file writer and Write function
            }

            // Update dict
            dict_update_heur(dict, gp);
            // Check if dictionary is full and if so use deletion heuristic
            deletion_heur(dict);
            // Update growing points pool
            growing_point_update_heur(gp_pool, &gpp_size);

            delete gp;
        }

        delete dict;
    }
}


