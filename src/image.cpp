#include "../headers/image.hpp"

#include <cstring>

Image::Image() : input_fname(""), output_fname(""), x_dim(0), y_dim(0), num_channels(0) {}

Image::Image(const std::string in_fname, const std::string ofname, ImageOperations op)
    : input_fname(in_fname), output_fname(ofname), op_type(op), x_dim(0), y_dim(0), num_channels(0) {}

Image::Image(const uint8_t* original_data, int width, int height) : x_dim(width), y_dim(height), num_channels(3) {
    const size_t NUM_VALUES = width * height * num_channels;
    pixel_data.assign(original_data, original_data + NUM_VALUES);
}

Image::~Image() {}