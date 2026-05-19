#include "../headers/image.hpp"

Image::Image() : input_fname(""), output_fname(""), x_dim(0), y_dim(0), num_channels(0) {}

Image::Image(const std::string in_fname, const std::string ofname, ImageOperations op)
    : input_fname(in_fname), output_fname(ofname), op_type(op), x_dim(0), y_dim(0), num_channels(0) {}

Image::~Image() {}