#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <types/pixel.hpp>
#include <vector>

#define NO_POSSIBLE_SYMBOLS 256

/**
 * @brief Calculates entropy based on the frequency dictionary of symbols.
 *
 * @param freq_dict An array containing the frequency of each symbol.
 * @param no_symbols The number of distinct symbols in the dictionary.
 * @return long double The calculated entropy value in bits.
 *
 * @details The entropy value is computed as the sum of weighted logarithms
 *          of the probabilities of symbol occurrences.
 *          If the frequency array is empty or the sum of frequencies is 0,
 *          the function returns 0.
 */
long double calculate_entropy(const uint32_t* freq_dict, uint64_t no_symbols);

/**
 * @brief Computes the Mean Squared Error (MSE) between two images.
 *
 * @param img1 The first image, represented as a matrix of pixels.
 * @param img2 The second image, represented as a matrix of pixels.
 * @return double The calculated MSE (the mean squared difference
 *                between pixel values of both images).
 *
 * @details Both images must have the same dimensions.
 *          If the dimensions differ, the result may be undefined.
 */
double calculate_mse(const std::vector<std::vector<Pixel>>& img1,
                     const std::vector<std::vector<Pixel>>& img2);

/**
 * @brief Calculates the Peak Signal-to-Noise Ratio (PSNR) based on the MSE value.
 *
 * @param mse The Mean Squared Error between the original and modified images.
 * @return double The PSNR value in decibels (dB).
 *
 * @details If the MSE value is 0 (no differences between images),
 *          the function returns infinity.
 */
double calculate_psnr(double mse);

/**
 * @brief Computes the Structural Similarity Index (SSIM) for three RGB channels.
 *
 * @param ssim_r The SSIM value for the red channel.
 * @param ssim_g The SSIM value for the green channel.
 * @param ssim_b The SSIM value for the blue channel.
 * @return double The SSIM value considering all RGB channels.
 *
 * @details The SSIM value is calculated as a weighted average of the
 *          SSIM values for the three RGB channels.
 */
double calculate_ssim(double ssim_r, double ssim_g, double ssim_b);

/**
 * @brief Computes the Structural Similarity Index (SSIM) for a selected image channel.
 *
 * @param img1 The first image, represented as a matrix of pixels.
 * @param img2 The second image, represented as a matrix of pixels.
 * @param channel The channel for which SSIM is calculated ('R', 'G', or 'B').
 * @param window_size The size of the comparison window (default is 8).
 * @return double The SSIM value for the selected image channel.
 *
 * @details The function divides the images into windows of the specified size
 *          and compares their local features.
 *          Both images must have the same dimensions.
 *          The window size should be adjusted to the resolution of the image.
 */
double calculate_ssim_channel(const std::vector<std::vector<Pixel>>& img1,
                              const std::vector<std::vector<Pixel>>& img2,
                              char channel, size_t window_size = 8);

