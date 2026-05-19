#ifndef _HEADERS_IMAGE_H_
#define _HEADERS_IMAGE_H_
#include <memory>
#include <string>
#include <tuple>
#include <vector>

enum class ImageOperations { SIMPLE_BLUR, GAUSSIAN_BLUR, SEGMENTATION, GRAYSCALE };
class Image {
   public:
    std::vector<unsigned char> data;
    int x_dim;
    int y_dim;
    int num_channels;
    std::string input_fname;
    std::string output_fname;
    ImageOperations op_type;

    Image();
    Image(const std::string ifname, const std::string ofname, ImageOperations op);
    ~Image();
};

#endif