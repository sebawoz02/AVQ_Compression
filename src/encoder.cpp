#include <cmath>
#include <encoder.hpp>
#include <types/growing_point.hpp>

void Encoder::encode(Image image)
{
  // Add pixel [0, 0] to growing points pool
  GP_pool gp_pool;
  gp_pool.add(new Growing_point(0, 0));
  // Init dictionary
  Dictionary* dict = dict_init_heur();
  dict->set_deletion_mode(deletion_heur);

  // While growing points pool has more elements
  while(gp_pool.size() > 0) {
    // Determine next growing point
    GP_pool_entry* gp_p = growing_heur(gp_pool);

    size_t common_block_idx;
    Block* picked_block;
    match_heur(*dict, tolerance, image, gp_p->gp, &common_block_idx,
               &picked_block);

    auto bits_to_transmit =
      static_cast<int8_t>(std::ceil(log2(static_cast<double>(dict->size()))));

    // Write common_block_idx to file on log2(dict.size) bits
    for(int8_t i = bits_to_transmit - 1; i >= 0; i--) {
      bool bit = (common_block_idx >> i) & 1;
      io_handler->write(bit);
    }
    // Update dict
    dict_update_heur(*dict, picked_block, gp_p->gp, image);
    delete picked_block;
    // Check if dictionary is full and if so use deletion heuristic
    heuristic::dict_deletion::deletion(*dict);
    // Update growing points pool
    growing_point_update_heur(image, gp_pool, gp_p);
  }

  delete dict;
}
