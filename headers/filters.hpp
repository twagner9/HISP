#ifndef _HEADERS_FILTERS_HPP_
#define _HEADERS_FILTERS_HPP_

#include "image.hpp"

extern "C" {
namespace ImageFilters {

void blur(Image *img, const int &blur_radius);
void gaussian_blur(Image *img, const int &blur_radius);
void grayscale(Image *img);
}  // namespace ImageFilters
}
#endif