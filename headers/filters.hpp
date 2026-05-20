#ifndef _HEADERS_FILTERS_HPP_
#define _HEADERS_FILTERS_HPP_

#include <algorithm>
#include <cmath>
#include <cstring>

#include "image.hpp"

namespace ImageFilters {

// TODO: make this gaussian by weighting each pixel's value
/**
 * @brief Apply a blur to an image.
 * @param img The image the user supplied that will be blurred. It is 2 * blur_radius + 1.
 * @param blur_radius How large the kernel will be when performing the blur
 * operation
 */
void blur(Image *img, const int &blur_radius) {
    const int NUM_BYTES{img->x_dim * img->y_dim * img->num_channels};
    constexpr int RGB_OFFSET{3};
    std::vector<unsigned char> res(NUM_BYTES);

    for (int i = 0; i < NUM_BYTES; i += RGB_OFFSET) {
        int pixel_idx{i / RGB_OFFSET};
        int x{pixel_idx % img->x_dim}, y{pixel_idx / img->x_dim};
        int pixels_summed{0};
        int r_val{0}, g_val{0}, b_val{0};

        for (int dy = y - blur_radius; dy < y + blur_radius; dy++) {
            for (int dx = x - blur_radius; dx < x + blur_radius; dx++) {
                if (dy >= 0 && dx >= 0 && dy < img->y_dim && dx < img->x_dim) {
                    int neighbor_idx = (dy * img->x_dim + dx) * RGB_OFFSET;
                    r_val += img->data[neighbor_idx];
                    g_val += img->data[neighbor_idx + 1];
                    b_val += img->data[neighbor_idx + 2];
                    ++pixels_summed;
                }
            }
        }
        if (pixels_summed > 0) {
            res[i] = r_val / pixels_summed;
            res[i + 1] = g_val / pixels_summed;
            res[i + 2] = b_val / pixels_summed;
        }
    }

    // crucial time saver -- no longer need the res vector, so tell compiler to
    // just reclaim it and do a constant time swap.
    img->data = std::move(res);
}

/**
 * @brief Applies a blur to an image using a pre-calculated Gaussian kernel.
 *
 * @param img The image to be blurred.
 * @param blur_radius The radius of the kernel used during blurring.
 */
void gaussian_blur(Image *img, const int &blur_radius) {
    // Basic setup: determine number of bytes contained in this image, since img->data is an unsigned char* array
    // containing 3 channels. result array should be the same size as the input.
    const int NUM_BYTES{img->x_dim * img->y_dim * img->num_channels};
    constexpr int RGB_OFFSET{3};
    std::vector<unsigned char> res(NUM_BYTES);
    float sigma{blur_radius / 2.0f};

    // FIXME: separable convolution will avoid 2D kernel, instead doing size x 1 and 1 x size kernels to reduce the number
    // mathematical operations being performed.
    // 1. Generate the kernel. Pre-made kernel will be applied as needed across all
    float kernel_sum{0.0f};
    int kernel_size{blur_radius * 2 + 1};
    std::vector<float> gaussian_kernel(kernel_size * kernel_size);

    for (int ky{-blur_radius}; ky <= blur_radius; ky++) {
        for (int kx{-blur_radius}; kx <= blur_radius; kx++) {
            float weight{std::exp(-(ky * ky + kx * kx) / (2 * sigma * sigma))};
            gaussian_kernel[(ky + blur_radius) * kernel_size + (kx + blur_radius)] = weight;
            kernel_sum += weight;
        }
    }

    // 2. Loop the input/output
    // (NUM_BYTES / 3) * (blur_radius * 2 + 1)^2 iterations...this is pretty expensive

    // 3. Get the specific pixel indices used for this pixel
    for (int y{0}; y < img->y_dim; y++) {
        for (int x{0}; x < img->x_dim; x++) {
            float r_val{0}, g_val{0}, b_val{0};

            for (int dy{-blur_radius}; dy < blur_radius; dy++) {
                for (int dx{-blur_radius}; dx < blur_radius; dx++) {
                    // Clamping is meant to prevent brightness changes during blur application
                    int ix{std::clamp(x + dx, 0, img->x_dim - 1)};
                    int iy{std::clamp(y + dy, 0, img->y_dim - 1)};

                    float weight = gaussian_kernel[(dy + blur_radius) * kernel_size + dx + blur_radius];
                    // int neighbor_idx{((dy + y) * img->x_dim + (dx + x)) * RGB_OFFSET};
                    int idx{(iy * img->x_dim + ix) * RGB_OFFSET};

                    r_val += img->data[idx] * weight;
                    g_val += img->data[idx + 1] * weight;
                    b_val += img->data[idx + 2] * weight;
                }
            }
            int res_idx{(y * img->x_dim + x) * RGB_OFFSET};
            res[res_idx] = (unsigned char)std::round((r_val / kernel_sum));
            res[res_idx + 1] = (unsigned char)std::round((g_val / kernel_sum));
            res[res_idx + 2] = (unsigned char)std::round((b_val / kernel_sum));
        }
    }

    img->data = std::move(res);
}

/**
 * @brief Converts an image from color to grayscale.
 *
 * @param img The image to converted to grayscale.
 */
void grayscale(Image *img) {
    const int NUM_BYTES = img->x_dim * img->y_dim * img->num_channels;
    int RGB_OFFSET = 3;

    for (int i = 0; i < NUM_BYTES; i += RGB_OFFSET) {
        int pixel_idx = i / RGB_OFFSET;
        float avg = (img->data[i] + img->data[i + 1] + img->data[i + 2]) / 3;
        img->data[i] = static_cast<unsigned char>(avg);
        img->data[i + 1] = static_cast<unsigned char>(avg);
        img->data[i + 2] = static_cast<unsigned char>(avg);
    }
}
}  // namespace ImageFilters

#endif