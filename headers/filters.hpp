#ifndef _HEADERS_FILTERS_HPP_
#define _HEADERS_FILTERS_HPP_

#include <cstring>

#include "image.hpp"

// TODO: make this gaussian by weighting each pixel's value
/**
 * @brief Apply a blur to an image.
 * @param img The image the user supplied that will be blurred.
 * @param blur_size How large the kernel will be when performing the blur
 * operation
 */
void blur(Image *img, const int &blur_size) {
    const int NUM_PIXELS = img->x_dim * img->y_dim * img->num_channels;
    int rgbOffset = 2;
    std::vector<unsigned char> res;
    res.reserve(NUM_PIXELS);

    for (int i = 0; i < NUM_PIXELS; i += 3) {
        int x{i % img->x_dim}, y{i / img->x_dim};
        int pixels_summed{0};
        int r_val{0}, g_val{0}, b_val{0};

        for (int row = y - blur_size; row < y + blur_size; row++) {
            for (int col = x - blur_size; col < x + blur_size; col++) {
                if (row >= 0 && col >= 0 && row < img->x_dim &&
                    col < img->y_dim) {
                    r_val += img->data[i];
                    g_val += img->data[i + 1];
                    b_val += img->data[i + 2];
                    ++pixels_summed;
                }
            }
        }
        res[i] = r_val / pixels_summed;
        res[i + 1] = g_val / pixels_summed;
        res[i + 2] = b_val / pixels_summed;
    }

    // crucial time saver -- no longer need the res vector, so tell compiler to
    // just reclaim it and do a constant time swap.
    img->data = std::move(res);
}

#endif