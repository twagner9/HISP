#ifndef _HEADERS_FILTERS_HPP_
#define _HEADERS_FILTERS_HPP_

#include <cstring>

#include "image.hpp"

namespace ImageFilters {

// TODO: make this gaussian by weighting each pixel's value
/**
 * @brief Apply a blur to an image.
 * @param img The image the user supplied that will be blurred. It is 2 * blur_size + 1.
 * @param blur_size How large the kernel will be when performing the blur
 * operation
 */
void blur(Image *img, const int &blur_size) {
    const int NUM_BYTES = img->x_dim * img->y_dim * img->num_channels;
    int rgbOffset = 3;
    std::vector<unsigned char> res(NUM_BYTES);

    for (int i = 0; i < NUM_BYTES; i += rgbOffset) {
        int pixel_idx = i / rgbOffset;
        int x{pixel_idx % img->x_dim}, y{pixel_idx / img->x_dim};
        int pixels_summed{0};
        int r_val{0}, g_val{0}, b_val{0};

        for (int row = y - blur_size; row < y + blur_size; row++) {
            for (int col = x - blur_size; col < x + blur_size; col++) {
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
}  // namespace ImageFilters

#endif