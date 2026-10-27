#include "../headers/filters_c_api.hpp"

#include <cstring>

#include "../headers/filters.hpp"

extern "C" {
HISP_API int image_grayscale(const uint8_t* input, uint8_t* output, int width, int height) {
    if (!input || !output) {
        return 1;
    }
    if (width <= 0 || height <= 0) {
        return 2;
    }

    constexpr int NUM_CHANNELS = 3;
    Image img(input, width, height);
    ImageFilters::grayscale(img);
    std::memcpy(output, img.pixel_data.data(), static_cast<size_t>(width) * height * NUM_CHANNELS * sizeof(uint8_t));

    return 0;
}

HISP_API int image_blur(const uint8_t* input, uint8_t* output, int width, int height, int blur_radius) {
    if (!input || !output) {
        return 1;
    }
    if (width <= 0 || height <= 0) {
        return 2;
    }
    if (blur_radius < 1) {
        return 3;
    }

    constexpr int NUM_CHANNELS = 3;
    Image img(input, width, height);
    std::fprintf(stderr, "About to execute image_blur\n");
    std::fflush(stderr);
    ImageFilters::blur(img, blur_radius);
    std::memcpy(output, img.pixel_data.data(), static_cast<size_t>(width) * height * NUM_CHANNELS * sizeof(uint8_t));
    return 0;
}

HISP_API int image_gaussian_blur(const uint8_t* input, uint8_t* output, int width, int height, int blur_radius) {
    if (!input || !output) {
        return 1;
    }
    if (width <= 0 || height <= 0) {
        return 2;
    }
    if (blur_radius < 1) {
        return 3;
    }

    constexpr int NUM_CHANNELS = 3;
    Image img(input, width, height);
    ImageFilters::gaussian_blur(img, blur_radius);
    std::memcpy(output, img.pixel_data.data(), static_cast<size_t>(width) * height * NUM_CHANNELS * sizeof(uint8_t));

    return 0;
}

HISP_API int image_adjust_brightness(const uint8_t* input, uint8_t* output, int width, int height, int delta) {
    if (!input || !output) {
        return 1;
    }

    if (width <= 0 || height <= 0) {
        return 2;
    }

    constexpr int NUM_CHANNELS = 3;
    Image img(input, width, height);
    ImageFilters::adjust_brightness(img, delta);
    std::memcpy(output, img.pixel_data.data(), static_cast<size_t>(width) * height * NUM_CHANNELS * sizeof(uint8_t));

    return 0;
}
}