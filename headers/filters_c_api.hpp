#ifndef _HEADERS_FILTERS_C_API_HPP
#define _HEADERS_FILTERS_C_API_HPP

#include <cstdint>

#if defined(_WIN32)
#define API __declspec(dllexport)
#else
#define API
#endif

extern "C" {

API int image_grayscale(const uint8_t* input, uint8_t* output, int width, int height);

API int image_blur(const uint8_t* input, uint8_t* output, int width, int height, int blur_radius);

API int image_gaussian_blur(const uint8_t* input, uint8_t* output, int width, int height, int blur_radius);
}

#endif