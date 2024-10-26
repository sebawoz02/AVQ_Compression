#include <types/image.hpp>

bool Image::encoded_at(size_t x, size_t y, size_t w, size_t h)
{
    for(size_t i = 0; i < w; i++)
    {
        for(size_t j = 0; j < h; j++)
        {
            if(encoded[x + i][y + j])
            {
                return true;
            }
        }
    }
    return false;
}