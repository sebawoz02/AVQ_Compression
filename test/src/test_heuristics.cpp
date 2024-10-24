#include <heuristics.hpp>
#include <iostream>
#include <test_common.hpp>

static void test_match();
static void test_dict_init();
static void test_growing();
static void test_gp_update();
static void test_dict_update();

void test_heuristics()
{
  test_match();
  test_dict_init();
  test_growing();
  test_dict_update();
  test_gp_update();

  std::cout << "Heuristics tests finished successfully!" << std::endl;
}

static void test_match()
{
  /**
   * Testcase 1:  The best match is the biggest block at the end of dict. (100% match - mse = 0)
   *
   *  - Image :
   *          / --------- / ---------- / --------- /
   *          |           |           |           |
   *          |     5     |     5     |     5     |
   *          |           |           |           |
   *          / --------- / ---------- / --------- /
   *          |           |           |           |
   *          |     6     |     5     |     5     |
   *          |           |           |           |
   *          / --------- / ---------- / --------- /
   *          |           |           |           |
   *          |     7     |     6     |     5     |
   *          |           |           |           |
   *          / --------- / ---------- / --------- /
   *  - GP:
   *      x - 1, y - 1
   *  - Dict:
   *      [[0]],[[1]],...,[[255]], [[5], [5]], [[5, 6]],
   *      [[5, 6], [5, 5]]
   *  - tolerance:
   *      0.9
   *
   */
  {
    // IN
    const size_t image_height = 3;
    const size_t image_width = 3;
    const double tolerance = 0.9;
    std::vector<std::vector<uint8_t>> pixels(std::vector<std::vector<uint8_t>>(
      image_width, std::vector<uint8_t>(image_height, 0)));
    pixels[0][0] = 5;
    pixels[0][1] = 6;
    pixels[0][2] = 7;
    pixels[1][0] = 5;
    pixels[1][1] = 5;
    pixels[1][2] = 6;
    pixels[2][0] = 5;
    pixels[2][1] = 5;
    pixels[2][2] = 5;
    Image image(pixels, image_width, image_height);
    assert(image.pixels[0][0] == 5);
    assert(image.pixels[0][1] == 6);
    assert(image.pixels[0][2] == 7);
    assert(image.pixels[1][0] == 5);
    assert(image.pixels[1][1] == 5);
    assert(image.pixels[1][2] == 6);
    assert(image.pixels[2][0] == 5);
    assert(image.pixels[2][1] == 5);
    assert(image.pixels[2][2] == 5);

    auto gp = new Growing_point(1, 1);
    Dictionary* dict = heuristic::dict_init::range_0_to_255();
    assert(dict != nullptr);
    assert(dict->size() == 256);
    for(uint16_t i = 0; i < 256; i++) {
      assert((*dict)[i] != nullptr);
      assert((*dict)[i]->pixels[0][0] == i);
    }

    std::vector<std::vector<uint8_t>> b1(
      std::vector<std::vector<uint8_t>>(2, std::vector<uint8_t>(1, 0)));
    b1[0][0] = 5;
    b1[1][0] = 5;
    dict->insert(new Block(2, 1, b1));

    std::vector<std::vector<uint8_t>> b2(
      std::vector<std::vector<uint8_t>>(1, std::vector<uint8_t>(2, 0)));
    b2[0][0] = 5;
    b2[0][1] = 6;
    dict->insert(new Block(1, 2, b2));

    std::vector<std::vector<uint8_t>> b3(
      std::vector<std::vector<uint8_t>>(2, std::vector<uint8_t>(2, 0)));
    b3[0][0] = 5;
    b3[0][1] = 6;
    b3[1][0] = 5;
    b3[1][1] = 5;
    dict->insert(new Block(2, 2, b3));
    assert(dict->size() == 259);

    // OUT
    size_t common_block_idx;
    Block* best_block;

    heuristic::match::top_left_mse(dict, tolerance, image, gp,
                                   &common_block_idx, &best_block);

    assert(common_block_idx == 258);
    assert(best_block != nullptr);
    assert(best_block->height == 2);
    assert(best_block->width == 2);
    assert(best_block->pixels[0][0] == 5);
    assert(best_block->pixels[0][1] == 6);
    assert(best_block->pixels[1][0] == 5);
    assert(best_block->pixels[1][1] == 5);

    assert(image.encoded[1][1] == true);
    assert(image.encoded[1][2] == true);
    assert(image.encoded[2][1] == true);
    assert(image.encoded[2][2] == true);

    delete dict;
    delete best_block;
    delete gp;
  }
  /**
   * Testcase 2:  There is no match correct match for this tolerance. 1x1 pixel should be picked.
   *
   *  - Image :
   *          / --------- / ---------- / --------- /
   *          |           |           |           |
   *          |     5     |     5     |     5     |
   *          |           |           |           |
   *          / --------- / ---------- / --------- /
   *          |           |           |           |
   *          |     6     |     255   |     5     |
   *          |           |           |           |
   *          / --------- / ---------- / --------- /
   *          |           |           |           |
   *          |     7     |     6     |     5     |
   *          |           |           |           |
   *          / --------- / ---------- / --------- /
   *  - GP:
   *      x - 1, y - 1
   *  - Dict:
   *      [[0]],[[1]],...,[[255]], [[5], [5]], [[5, 6]],
   *      [[5, 6], [5, 255]]
   *  - tolerance:
   *      0.9
   *
   */
  {
    // IN
    const size_t image_height = 3;
    const size_t image_width = 3;
    const double tolerance = 0.9;
    std::vector<std::vector<uint8_t>> pixels(std::vector<std::vector<uint8_t>>(
      image_width, std::vector<uint8_t>(image_height, 0)));
    pixels[0][0] = 5;
    pixels[0][1] = 6;
    pixels[0][2] = 7;
    pixels[1][0] = 5;
    pixels[1][1] = 255;
    pixels[1][2] = 6;
    pixels[2][0] = 5;
    pixels[2][1] = 5;
    pixels[2][2] = 5;
    Image image(pixels, image_width, image_height);
    assert(image.pixels[0][0] == 5);
    assert(image.pixels[0][1] == 6);
    assert(image.pixels[0][2] == 7);
    assert(image.pixels[1][0] == 5);
    assert(image.pixels[1][1] == 255);
    assert(image.pixels[1][2] == 6);
    assert(image.pixels[2][0] == 5);
    assert(image.pixels[2][1] == 5);
    assert(image.pixels[2][2] == 5);

    auto gp = new Growing_point(1, 1);
    Dictionary* dict = heuristic::dict_init::range_0_to_255();
    assert(dict != nullptr);
    assert(dict->size() == 256);
    for(uint16_t i = 0; i < 256; i++) {
      assert((*dict)[i] != nullptr);
      assert((*dict)[i]->pixels[0][0] == i);
    }

    std::vector<std::vector<uint8_t>> b1(
      std::vector<std::vector<uint8_t>>(2, std::vector<uint8_t>(1, 0)));
    b1[0][0] = 5;
    b1[1][0] = 5;
    dict->insert(new Block(2, 1, b1));

    std::vector<std::vector<uint8_t>> b2(
      std::vector<std::vector<uint8_t>>(1, std::vector<uint8_t>(2, 0)));
    b2[0][0] = 5;
    b2[0][1] = 6;
    dict->insert(new Block(1, 2, b2));

    std::vector<std::vector<uint8_t>> b3(
      std::vector<std::vector<uint8_t>>(2, std::vector<uint8_t>(2, 0)));
    b3[0][0] = 5;
    b3[0][1] = 6;
    b3[1][0] = 5;
    b3[1][1] = 255;
    dict->insert(new Block(2, 2, b3));
    assert(dict->size() == 259);

    // OUT
    size_t common_block_idx;
    Block* best_block;

    heuristic::match::top_left_mse(dict, tolerance, image, gp,
                                   &common_block_idx, &best_block);

    assert(common_block_idx == 255);
    assert(best_block != nullptr);
    assert(best_block->height == 1);
    assert(best_block->width == 1);
    assert(best_block->pixels[0][0] == 255);

    assert(image.encoded[1][1] == true);

    delete dict;
    delete best_block;
    delete gp;
  }
}

static void test_dict_init()
{
  Dictionary* dict = heuristic::dict_init::range_0_to_255();

  assert(dict != nullptr);
  assert(dict->size() == 256);

  for(uint16_t i = 0; i < 256; i++) {
    assert((*dict)[i] != nullptr);
    assert((*dict)[i]->pixels[0][0] == i);
  }

  delete dict;
}

static void test_growing()
{
  // {1, 2}, {3, 1}, {2, 1}, {0, 2}
  GP_pool* gpp = new GP_pool();
  auto gp1 = new Growing_point(1, 2);
  auto gp2 = new Growing_point(3, 1);
  auto gp3 = new Growing_point(2, 1);
  auto gp4 = new Growing_point(0, 2);
  gpp->add(gp1);
  gpp->add(gp2);
  gpp->add(gp3);
  gpp->add(gp4);

  // WAVE
  {
    Growing_point* gp = heuristic::growing::wave(gpp);
    assert(gp != nullptr);
    assert(gp->x == 0);
    assert(gp->y == 2);
    assert(gp == gp4);
  }

  // DIAGONAL
  {
    Growing_point* gp = heuristic::growing::diagonal(gpp);
    assert(gp != nullptr);
    assert(gp->x == 1);
    assert(gp->y == 2);
    assert(gp == gp1);
  }

  // LIFO
  {
    Growing_point* gp = heuristic::growing::lifo(gpp);
    assert(gp != nullptr);
    assert(gp->x == 0);
    assert(gp->y == 2);
    assert(gp == gp4);
  }

  delete gpp;
}

static void test_gp_update()
{
  /**
   * Testcase 1:  cur_gp should be removed from GPP. [0,1] and [2,0] should be
   *added.
   *
   *  - Image :
   *          / --------- / ---------- / --------- /
   *          |           |           |           |
   *          |    true   |    true   |   false   |
   *          |           |           |           |
   *          / --------- / ---------- / --------- /
   *          |           |           |           |
   *          |   false   |   false   |   false   |
   *          |           |           |           |
   *          / --------- / ---------- / --------- /
   *          |           |           |           |
   *          |   false   |   false   |   false   |
   *          |           |           |           |
   *          / --------- / ---------- / --------- /
   *  - GPP:
   *    {[1, 0]}
   *  - size: 1
   *  - cur_gp: [1, 0]
   **/
  {
    const size_t image_height = 3;
    const size_t image_width = 3;
    std::vector<std::vector<uint8_t>> pixels(std::vector<std::vector<uint8_t>>(
      image_width, std::vector<uint8_t>(image_height, 0)));
    Image image(pixels, image_width, image_height);
    image.encoded[0][0] = true;
    image.encoded[1][0] = true;

    auto* gpp = new GP_pool();
    auto* cur_gp = new Growing_point(1, 0);
    gpp->add(cur_gp);
    size_t gpp_size = 1;

    assert((*gpp)[0] == cur_gp);

    heuristic::gp_update::first_from_left(image, gpp, &gpp_size, cur_gp);

    assert(gpp_size == 2);
    assert((*gpp)[0]->x == 2);
    assert((*gpp)[0]->y == 0);
    assert((*gpp)[1]->x == 0);
    assert((*gpp)[1]->y == 1);

    delete gpp;
  }

  /**
   * Testcase 2:  cur_gp should be removed from GPP. Only [1, 2] should be
   *added.
   *
   *  - Image :
   *          / --------- / ---------- / --------- /
   *          |           |           |           |
   *          |    true   |    true   |    true   |
   *          |           |           |           |
   *          / --------- / ---------- / --------- /
   *          |           |           |           |
   *          |   true    |   false   |   false   |
   *          |           |           |           |
   *          / --------- / ---------- / --------- /
   *          |           |           |           |
   *          |   true    |   false   |   false   |
   *          |           |           |           |
   *          / --------- / ---------- / --------- /
   *  - GPP:
   *    {[0, 2], [1, 1]}
   *  - size: 2
   *  - cur_gp: [0, 2]
   **/
  {
    const size_t image_height = 3;
    const size_t image_width = 3;
    std::vector<std::vector<uint8_t>> pixels(std::vector<std::vector<uint8_t>>(
      image_width, std::vector<uint8_t>(image_height, 0)));
    Image image(pixels, image_width, image_height);
    image.encoded[0][0] = true;
    image.encoded[1][0] = true;
    image.encoded[2][0] = true;
    image.encoded[0][1] = true;
    image.encoded[0][2] = true;

    auto* gpp = new GP_pool();
    auto* cur_gp = new Growing_point(0, 2);
    auto* another_gp = new Growing_point(1, 1);
    gpp->add(cur_gp);
    gpp->add(another_gp);
    size_t gpp_size = 2;

    assert((*gpp)[0] == cur_gp);
    assert((*gpp)[1] == another_gp);

    heuristic::gp_update::first_from_left(image, gpp, &gpp_size, cur_gp);

    assert(gpp_size == 2);
    assert((*gpp)[0] == another_gp);
    assert((*gpp)[1]->x == 1);
    assert((*gpp)[1]->y == 2);

    delete gpp;
  }
}

static void test_dict_update()
{
  // One Row
  {
    /**
     * - Image :
     *          / --------- / ---------- / --------- /
     *          |           |           |           |
     *          |     5     |     5     |     5     |
     *          |           |           |           |
     *          / --------- / ---------- / --------- /
     *          |           |           |           |
     *          |     6     |     5     |     5     |
     *          |           |           |           |
     *          / --------- / ---------- / --------- /
     *          |           |           |           |
     *          |     7     |     6     |     5     |
     *          |           |           |           |
     *          / --------- / ---------- / --------- /
     */
    const size_t image_height = 3;
    const size_t image_width = 3;
    std::vector<std::vector<uint8_t>> pixels(std::vector<std::vector<uint8_t>>(
      image_width, std::vector<uint8_t>(image_height, 0)));
    pixels[0][0] = 5;
    pixels[0][1] = 6;
    pixels[0][2] = 7;
    pixels[1][0] = 5;
    pixels[1][1] = 255;
    pixels[1][2] = 6;
    pixels[2][0] = 5;
    pixels[2][1] = 5;
    pixels[2][2] = 5;
    Image image(pixels, image_width, image_height);

    Dictionary* dict = heuristic::dict_init::range_0_to_255();
    assert(dict != nullptr);
    assert(dict->size() == 256);

    std::vector<std::vector<uint8_t>> p(
      std::vector<std::vector<uint8_t>>(1, std::vector<uint8_t>(1, 5)));
    assert(p[0][0] == 5);
    auto* picked_block = new Block(1, 1, p);
    auto* gp = new Growing_point(1, 1);

    heuristic::dict_update::one_row(dict, picked_block, gp, image);

    assert(dict->size() == 257);
    assert((*dict)[256]->width == 1);
    assert((*dict)[256]->height == 2);
    assert((*dict)[256]->pixels[0][0] == 5);
    assert((*dict)[256]->pixels[0][1] == 5);

    delete dict;
    delete gp;
    delete picked_block;
  }

  // One Column
  {
    const size_t image_height = 3;
    const size_t image_width = 3;
    std::vector<std::vector<uint8_t>> pixels(std::vector<std::vector<uint8_t>>(
      image_width, std::vector<uint8_t>(image_height, 0)));
    pixels[0][0] = 5;
    pixels[0][1] = 6;
    pixels[0][2] = 7;
    pixels[1][0] = 5;
    pixels[1][1] = 255;
    pixels[1][2] = 6;
    pixels[2][0] = 5;
    pixels[2][1] = 5;
    pixels[2][2] = 5;
    Image image(pixels, image_width, image_height);

    Dictionary* dict = heuristic::dict_init::range_0_to_255();
    assert(dict != nullptr);
    assert(dict->size() == 256);

    std::vector<std::vector<uint8_t>> p(
      std::vector<std::vector<uint8_t>>(1, std::vector<uint8_t>(1, 5)));
    assert(p[0][0] == 5);
    auto* picked_block = new Block(1, 1, p);
    auto* gp = new Growing_point(1, 1);

    heuristic::dict_update::one_column(dict, picked_block, gp, image);

    assert(dict->size() == 257);
    assert((*dict)[256]->width == 2);
    assert((*dict)[256]->height == 1);
    assert((*dict)[256]->pixels[0][0] == 6);
    assert((*dict)[256]->pixels[1][0] == 5);

    delete dict;
    delete gp;
    delete picked_block;
  }
}