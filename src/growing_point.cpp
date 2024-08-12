#include <growing_point.hpp>

Growing_point::Growing_point(size_t _x, size_t _y, size_t width, size_t height, std::vector<std::vector<uint8_t>>& src) {
    x = _x;
    y = _y;

    std::vector<std::vector<uint8_t>> sub_map(width, std::vector<uint8_t>(height, 0));
    for(size_t i = x; i < x + width; i++)
    {
        for(size_t j = y; j < y + height; j++)
        {
            sub_map[i][j] = src[i][j];
        }
    }

    block = new Block(width, height, sub_map);
}
