#include <analyzer/stats.hpp>
#include <cmath>

// Constants for SSIM
static const double C1 = 0.01 * 0.01;
static const double C2 = 0.03 * 0.03;

static double mean(const std::vector<std::vector<Pixel>>& img, size_t x,
                   size_t y, size_t window_size, char channel)
{
  double sum = 0.0;
  for(size_t i = 0; i < window_size; ++i) {
    for(size_t j = 0; j < window_size; ++j) {
      switch(channel) {
      case 'r':
        sum += img[x + i][y + j].red;
        break;
      case 'g':
        sum += img[x + i][y + j].green;
        break;
      case 'b':
        sum += img[x + i][y + j].blue;
        break;
      }
    }
  }
  return sum / (window_size * window_size);
}

static double variance(const std::vector<std::vector<Pixel>>& img, size_t x,
                       size_t y, size_t window_size, double mean, char channel)
{
  double sum = 0.0;
  for(size_t i = 0; i < window_size; ++i) {
    for(size_t j = 0; j < window_size; ++j) {
      double value = 0;
      switch(channel) {
      case 'r':
        value = img[x + i][y + j].red;
        break;
      case 'g':
        value = img[x + i][y + j].green;
        break;
      case 'b':
        value = img[x + i][y + j].blue;
        break;
      }
      sum += (value - mean) * (value - mean);
    }
  }
  return sum / (window_size * window_size);
}

static double covariance(const std::vector<std::vector<Pixel>>& img1,
                         const std::vector<std::vector<Pixel>>& img2, int x,
                         int y, int window_size, double mean1, double mean2,
                         char channel)
{
  double sum = 0.0;
  for(int i = 0; i < window_size; ++i) {
    for(int j = 0; j < window_size; ++j) {
      double value1 = 0, value2 = 0;
      switch(channel) {
      case 'r':
        value1 = img1[x + i][y + j].red;
        value2 = img2[x + i][y + j].red;
        break;
      case 'g':
        value1 = img1[x + i][y + j].green;
        value2 = img2[x + i][y + j].green;
        break;
      case 'b':
        value1 = img1[x + i][y + j].blue;
        value2 = img2[x + i][y + j].blue;
        break;
      }
      sum += (value1 - mean1) * (value2 - mean2);
    }
  }
  return sum / (window_size * window_size);
}

long double calculate_entropy(const uint32_t* freq_dict,
                              const uint64_t no_symbols)
{
  long double ent =
    0.0; // H = -1 * sum(for(i in freq_dict){ p(i) * log2(p(i)) })
  const auto no_sym = static_cast<long double>(no_symbols);

  for(size_t i = 0; i < NO_POSSIBLE_SYMBOLS; i++) {
    if(freq_dict[i] == 0)
      continue;
    const long double p =
      static_cast<long double>(freq_dict[i]) / no_sym; // p(freq_dict[i])
    ent -= p * log2l(p);
  }
  return ent;
}

double calculate_mse(const std::vector<std::vector<Pixel>>& img1,
                     const std::vector<std::vector<Pixel>>& img2)
{
  size_t height = img1.size();
  size_t width = img1[0].size();
  double mse = 0.0;

  for(size_t i = 0; i < height; ++i) {
    for(size_t j = 0; j < width; ++j) {
      // Calculate the squared difference for each channel
      double diff_r = img1[i][j].red - img2[i][j].red;
      double diff_g = img1[i][j].green - img2[i][j].green;
      double diff_b = img1[i][j].blue - img2[i][j].blue;

      // Sum of squared differences for this pixel
      mse += (diff_r * diff_r + diff_g * diff_g + diff_b * diff_b) / 3.0;
    }
  }

  mse /= (width * height);
  return mse;
}

double calculate_psnr(double mse)
{
  const double max_pixel_value = 255.0;
  return 10.0 * std::log10((max_pixel_value * max_pixel_value) / mse);
}

double calculate_ssim_channel(const std::vector<std::vector<Pixel>>& img1,
                              const std::vector<std::vector<Pixel>>& img2,
                              char channel, size_t window_size)
{
  size_t height = img1.size();
  size_t width = img1[0].size();
  double ssim_sum = 0.0;
  size_t num_windows = 0;

  for(size_t i = 0; i <= height - window_size; i += window_size) {
    for(size_t j = 0; j <= width - window_size; j += window_size) {
      double mean1 = mean(img1, i, j, window_size, channel);
      double mean2 = mean(img2, i, j, window_size, channel);
      double var1 = variance(img1, i, j, window_size, mean1, channel);
      double var2 = variance(img2, i, j, window_size, mean2, channel);
      double covar =
        covariance(img1, img2, i, j, window_size, mean1, mean2, channel);

      // SSIM for this window
      double ssim = ((2 * mean1 * mean2 + C1) * (2 * covar + C2)) /
                    ((mean1 * mean1 + mean2 * mean2 + C1) * (var1 + var2 + C2));
      ssim_sum += ssim;
      ++num_windows;
    }
  }
  return ssim_sum / num_windows;
}

double calculate_ssim(double ssim_r, double ssim_g, double ssim_b)
{
  return (ssim_r + ssim_g + ssim_b) / 3.0;
}
