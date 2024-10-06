#include <encoder.hpp>
#include <growing_point.hpp>
#include <cmath>
#include <output_writer.hpp>

namespace encoder {
    void Encoder::encode(const std::vector<std::vector<Pixel>>& image)
    {
        const size_t width = image.size();
        const size_t height = image.size();
        std::vector<std::vector<uint8_t>> red(std::vector<std::vector<uint8_t>>(width, std::vector<uint8_t>(height, 0)));
        std::vector<std::vector<uint8_t>> green(std::vector<std::vector<uint8_t>>(width, std::vector<uint8_t>(height, 0)));
        std::vector<std::vector<uint8_t>> blue(std::vector<std::vector<uint8_t>>(width, std::vector<uint8_t>(height, 0)));

        for(size_t i = 0; i < width; i++)
        {
            for(size_t j = 0; j < height; j++)
            {
                red[i][j] = image[i][j].red;
                green[i][j] = image[i][j].green;
                blue[i][j] = image[i][j].blue;
            }
        }

        adaptive_vector_quantization(red);
        adaptive_vector_quantization(green);
        adaptive_vector_quantization(blue);
    }

    void Encoder::adaptive_vector_quantization(std::vector<std::vector<uint8_t>>& image)
    {
        output_writer::OutWriter out_writer(*out_file);

        // Add pixel [0, 0] to growing points pool
        auto* init_gp = new Growing_point(0, 0, 1, 1, image);
        std::vector<Growing_point*> gp_pool{};
        gp_pool.push_back(init_gp);

        size_t gpp_size = 1;

        // Init dictionary
        Dictionary* dict = dict_init_heur();

        // While growing points pool has more elements
        while (gpp_size > 0)
        {
            // Determine next growing point
            Growing_point* gp = growing_heur(gp_pool);
            gpp_size--; // Current gp removed from growing points pool

            size_t common_block_idx;
            find_common_block(dict, gp, &common_block_idx);

            auto bits_to_transmit = static_cast<size_t>(
                    std::floor(log2(static_cast<double>(dict->size))));

            // Write common_block_idx to file on log2(dict.size) bits
            for(int8_t i = bits_to_transmit - 1; i >= 0; i--)
            {
                bool bit = (common_block_idx >> i) & 1;
                out_writer.write(bit);
            }

            // Update dict
            dict_update_heur(dict, gp, image);
            // Check if dictionary is full and if so use deletion heuristic
            deletion_heur(dict);
            // Update growing points pool
            growing_point_update_heur(gp_pool, &gpp_size, gp);

            delete gp;
        }

        delete dict;
    }

    void Encoder::find_common_block(Dictionary* dict, Growing_point* current_gp, size_t* common_block_idx)
    {
        if(current_gp->block->width == 1 && current_gp->block->height == 1)
        {
            *common_block_idx = current_gp->block->pixels[0][0];
            return;
        }

        double best_match = 999.0;
        size_t best_i = 0;
        for(size_t i = 256; i < dict->size; i++)
        {
            const double match = match_heur(current_gp->block, dict->entries[i]);
            if(match < tolerance) // Is this the right way to do this??
            {
                *common_block_idx = i;
                return;
            }
            if(best_match > match)
            {
                best_match = match;
                best_i = i;
            }
        }
        *common_block_idx = best_i;
    }
}
