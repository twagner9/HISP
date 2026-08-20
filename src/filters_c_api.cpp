#include "../headers/filters_c_api.hpp"

#include <cstring>

#include "../headers/filters.hpp"

#if defined(_WIN32)
#define API __declspec(dllexport)
#else
#define API
#endif

extern "C" {
API int image_grayscale(const uint8_t* input, uint8_t* output, int width, int height) {
    if (!input || !output) {
        return 1;
    }
    if (width <= 0 || height <= 0) {
        return 2;
    }

    constexpr int NUM_CHANNELS = 3;
    Image img(input, width, height);
    ImageFilters::grayscale(&img);
    std::memcpy(output, img.pixelData.data(), static_cast<size_t>(width) * height * NUM_CHANNELS * sizeof(uint8_t));

    return 0;
}

API int image_blur(const uint8_t* input, uint8_t* output, int width, int height, int blur_radius) {
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
    ImageFilters::blur(&img, blur_radius);
    std::memcpy(output, img.pixelData.data(), static_cast<size_t>(width) * height * NUM_CHANNELS * sizeof(uint8_t));
    return 0;
}

API int image_gaussian_blur(const uint8_t* input, uint8_t* output, int width, int height, int blur_radius) {
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
    ImageFilters::gaussian_blur(&img, blur_radius);
    std::memcpy(output, img.pixelData.data(), static_cast<size_t>(width) * height * NUM_CHANNELS * sizeof(uint8_t));

    return 0;
}
}