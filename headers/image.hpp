#ifndef _HEADERS_IMAGE_H_
#define _HEADERS_IMAGE_H_
#include <memory>
#include <string>
#include <tuple>
#include <vector>

class Image {
   public:
    std::vector<unsigned char> data;
    int x_dim;
    int y_dim;
    int num_channels;
    std::string input_fname;
    std::string output_fname;

    Image();
    ~Image();
};

#endif