#include <growing_point.hpp>

Growing_point::Growing_point(size_t _x, size_t _y, size_t width, size_t height, ColorMap *src) {
    x = _x;
    y = _y;

    auto* sub_map = new ColorMap(width, std::vector<uint8_t>(height));
    for(size_t i = x; i < x + width; i++)
    {
        for(size_t j = y; j < y + height; j++)
        {
            sub_map[i][j] = src[i][j];
        }
    }

    block = new Block(width, height, sub_map);
}

Growing_point::~Growing_point() {
    delete block;
}
