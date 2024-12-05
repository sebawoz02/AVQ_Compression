#pragma once

#include <types/dictionary.hpp>
#include <types/gp_pool.hpp>
#include <types/growing_point.hpp>
#include <types/image.hpp>

namespace heuristic {

  ///> Match heuristic function type
  using mh_t = void (*)(Dictionary&, double, Image&, Growing_point*, size_t*,
                        Block**);

  ///> Dictionary Init heuristic function type
  using idh_t = Dictionary* (*)();

  ///> Growing Point Pool Update heuristic function type
  using gpuh_t = void (*)(Image&, GP_pool*, Growing_point*);

  ///> Growing heuristic function type
  using gh_t = Growing_point* (*)(GP_pool*);

  ///> Dictionary Update heuristic function type
  using duh_t = void (*)(Dictionary*, Block*, Growing_point*, Image&);

  ///> Dictionary entry deletion heuristic function type
  using dh_t = void (*)(Dictionary*);

  /*
   * @param dict A reference to the dictionary used for vector quantization.
   *             It contains the set of representative blocks.
   * @param tolerance The allowed threshold for the match tolerance. A lower value
   *                  results in stricter matching, while a higher value allows
   *                  more leniency.
   * @param image A reference to the input image being processed. This contains the
   *              current block to be compared with the dictionary entries.
   * @param current_gp A pointer to the `Growing_point` representing the current
   *                   block being processed in the image.
   * @param[out] common_block_idx A pointer to a variable that will hold the index of
   *                         the matching block from the dictionary.
   * @param[out] picked_block A double pointer to a `Block` object that will be updated
   *                     with the block chosen from the dictionary.
   */
  namespace match {
    /**
     * @brief Computes the Mean Squared Error (MSE) to select a block from the dictionary
     *        that best matches the current block of the image.
     */
    void mse(Dictionary& dict, double tolerance, Image& image,
             Growing_point* current_gp, size_t* common_block_idx,
             Block** picked_block);
    /**
     * @brief Computes the Maximum Squared Error to select a block from the dictionary
     *        that best matches the current block of the image.
     */
    void max_se(Dictionary& dict, double tolerance, Image& image,
                Growing_point* current_gp, size_t* common_block_idx,
                Block** picked_block);
    /**
     * @brief Computes the Euclidean Distance to select a block from the dictionary
     *        that best matches the current block of the image.
     */
    void euclidean(Dictionary& dict, double tolerance, Image& image,
                   Growing_point* current_gp, size_t* common_block_idx,
                   Block** picked_block);
  } // namespace match

  namespace dict_init {
    /**
     * @brief Creates and initializes a dictionary containing 1x1 blocks with values in the range [0, 255].
     *
     * @return Dictionary* A pointer to a newly created dictionary with entries
     *                     representing all values from 0 to 255.
     */
    Dictionary* range_0_to_255();
  } // namespace dict_init

  namespace gpp_update {
    void first_from_left(Image& image, GP_pool* growing_points,
                         Growing_point* cur_gp);
  }

  /**
   * @param gp_pool A pointer to the `GP_pool`, which manages the pool of available growing points.
   * @return Growing_point* A pointer to the selected GP to be processed.
   */
  namespace growing {
    /**
     * @brief Selects the next GP to process using the "wave" heuristic.
     */
    Growing_point* wave(GP_pool* gp_pool);
    /**
     * @brief Selects the next GP to process using the "diagonal" heuristic.
     */
    Growing_point* diagonal(GP_pool* gp_pool);
    /**
     * @brief Selects the next GP to process using the "lifo" heuristic.
     */
    Growing_point* lifo(GP_pool* gp_pool);
    // circular coverage
  } // namespace growing

  /**
   * @param dict A pointer to the `Dictionary` to be updated.
   * @param picked_block A pointer to the `Block` object selected during the encoding process.
   * @param gp A pointer to the `Growing_point` associated with the current operation.
   * @param image A reference to the `Image` object being processed.
   * */
  namespace dict_update {
    /**
     * @brief Updates the dictionary by adding a new column of data based on the picked block and growing point.
     */
    void one_column(Dictionary* dict, Block* picked_block, Growing_point* gp,
                    Image& image);
    /**
     * @brief Updates the dictionary by adding a new row of data based on the picked block and growing point.
     */
    void one_row(Dictionary* dict, Block* picked_block, Growing_point* gp,
                 Image& image);
    /**
     * @brief Updates the dictionary by adding a new column and row of data based on the picked block and growing point.
     */
    void one_column_one_row(Dictionary* dict, Block* picked_block,
                            Growing_point* gp, Image& image);
  } // namespace dict_update

  namespace dict_deletion {
    void deletion(Dictionary& dict);
  }
} // namespace heuristic