#include <pixel.hpp>

[[nodiscard]] bool operator==(const Pixel& p1, const Pixel& p2)
{
    return p1.red == p2.red && p1.green == p2.green && p1.blue == p2.blue;
}