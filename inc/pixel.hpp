#pragma once

#include <cstdint>

struct Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

[[nodiscard]] bool operator==(const Pixel& p1, const Pixel& p2);