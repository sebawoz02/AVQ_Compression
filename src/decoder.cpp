#include <cmath>
#include <decoder.hpp>

static void anchor_gp(Growing_point* gp, Block* block, Image& image);

static void anchor_gp(Growing_point* gp, Block* block, Image& image)

{
  for(size_t i = 0; i < block->width; i++) {
    for(size_t j = 0; j < block->height; j++) {
      image.pixels[gp->x + i][gp->y + j] = block->pixels[i][j];
      image.encoded[gp->x + i][gp->y + j] = true;
    }
  }
}

std::vector<std::vector<Pixel>> Decoder::decode(size_t width, size_t height)
{
  Image red = adaptive_vector_quantization(width, height);
  Image green = adaptive_vector_quantization(width, height);
  Image blue = adaptive_vector_quantization(width, height);

  std::vector<std::vector<Pixel>> image(std::vector<std::vector<Pixel>>(
    width, std::vector<Pixel>(height, Pixel(0, 0, 0))));

  for(size_t i = 0; i < width; i++) {
    for(size_t j = 0; j < height; j++) {
      image[i][j].red = red.pixels[i][j];
      image[i][j].green = green.pixels[i][j];
      image[i][j].blue = blue.pixels[i][j];
    }
  }

  return image;
}

Image Decoder::adaptive_vector_quantization(size_t width, size_t height)
{
  std::vector<std::vector<uint8_t>> pixels(
    std::vector<std::vector<uint8_t>>(width, std::vector<uint8_t>(height, 0)));
  Image image(pixels, width, height);

  // Init growing points pool with [0, 0]
  auto* gpp = new GP_pool();
  gpp->add(new Growing_point(0, 0));

  // Init Dict
  Dictionary* dict = dict_init_heur();

  while(gpp->size() > 0) {
    Growing_point* gp = growing_heur(gpp);

    auto bits_to_receive =
      static_cast<int8_t>(std::ceil(log2(static_cast<double>(dict->size()))));

    uint16_t block_idx = io_handler->read_bits(bits_to_receive);
    Block* b = (*dict)[block_idx];
    // Anchor b to the current gp, in order to reconstruct the output image
    anchor_gp(gp, b, image);

    dict_update_heur(dict, b, gp, image);
    deletion_heur(dict);
    growing_point_update_heur(image, gpp, gp);
  }

  delete gpp;
  delete dict;
  return image;
}
