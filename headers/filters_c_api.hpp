#ifndef _HEADERS_FILTERS_C_API_HPP
#define _HEADERS_FILTERS_C_API_HPP

#include <cstdint>

#if defined(_WIN32)
#define API __declspec(dllexport)
#else
#define HISP_API
#endif

extern "C" {

HISP_API int image_grayscale(const uint8_t* input, uint8_t* output, int width, int height);

HISP_API int image_blur(const uint8_t* input, uint8_t* output, int width, int height, int blur_radius);

HISP_API int image_gaussian_blur(const uint8_t* input, uint8_t* output, int width, int height, int blur_radius);

HISP_API int image_adjust_brightness(const uint8_t* input, uint8_t* output, int width, int height, int delta);

HISP_API int image_simple_threshold(const uint8_t* input, uint8_t* output, int width, int height, int threshold);
}

#endif