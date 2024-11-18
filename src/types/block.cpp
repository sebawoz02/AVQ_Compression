#include <types/block.hpp>

void Block::mean_and_variance(double& mean, double& variance) {
    double _mean = 0.0;
    auto size = static_cast<double>(width * height);

    for (size_t i = 0; i < width; ++i) {
        for (size_t j = 0; j < height; ++j) {
            _mean += pixels[i][j] / size;
        }
    }

   double _variance = 0.0;
    for (size_t i = 0; i < width; ++i) {
        for (size_t j = 0; j < height; ++j) {
            double diff = static_cast<double>(pixels[i][j]) - _mean;
            _variance = (diff * diff) / size;
        }
    }
    mean = _mean;
    variance = _variance;
}