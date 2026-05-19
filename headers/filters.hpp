#ifndef _HEADERS_FILTERS_HPP_
#define _HEADERS_FILTERS_HPP_

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
    const int NUM_BYTES = img->x_dim * img->y_dim * img->num_channels;
    int rgbOffset = 3;
    std::vector<unsigned char> res(NUM_BYTES);

    for (int i = 0; i < NUM_BYTES; i += rgbOffset) {
        int pixel_idx = i / rgbOffset;
        int x{pixel_idx % img->x_dim}, y{pixel_idx / img->x_dim};
        int pixels_summed{0};
        int r_val{0}, g_val{0}, b_val{0};

        for (int row = y - blur_radius; row < y + blur_radius; row++) {
            for (int col = x - blur_radius; col < x + blur_radius; col++) {
                if (row >= 0 && col >= 0 && row < img->y_dim && col < img->x_dim) {
                    int neighbor_idx = (row * img->x_dim + col) * rgbOffset;
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

void gaussian_blur(Image *img, const int &blur_radius) {
    const int NUM_BYTES = img->x_dim * img->y_dim * img->num_channels;
    int rgbOffset = 3;
    std::vector<unsigned char> res(NUM_BYTES);
    float sigma = blur_radius / 2;

    for (int i = 0; i < NUM_BYTES; i += rgbOffset) {
        int pixel_idx = i / rgbOffset;
        int x{pixel_idx % img->x_dim}, y{pixel_idx / img->x_dim};
        int pixels_summed{0};
        float r_val{0}, g_val{0}, b_val{0};
        const double euler_const = std::exp(1.0);

        for (int row = y - blur_radius; row < y + blur_radius; row++) {
            for (int col = x - blur_radius; col < x + blur_radius; col++) {
                if (row >= 0 && col >= 0 && row < img->y_dim && col < img->x_dim) {
                    int weight = std::pow(euler_const, -((row * row + col * col) / (2 * sigma)));
                    int neighbor_idx = (row * img->x_dim + col) * rgbOffset;
                    r_val += img->data[neighbor_idx] * weight;
                    g_val += img->data[neighbor_idx + 1] * weight;
                    b_val += img->data[neighbor_idx + 2] * weight;
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
}

void grayscale(Image *img) {
    const int NUM_BYTES = img->x_dim * img->y_dim * img->num_channels;
    int rgbOffset = 3;

    for (int i = 0; i < NUM_BYTES; i += rgbOffset) {
        int pixel_idx = i / rgbOffset;
        float avg = (img->data[i] + img->data[i + 1] + img->data[i + 2]) / 3;
        img->data[i] = static_cast<unsigned char>(avg);
        img->data[i + 1] = static_cast<unsigned char>(avg);
        img->data[i + 2] = static_cast<unsigned char>(avg);
    }
}
}  // namespace ImageFilters

#endif