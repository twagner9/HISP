#ifndef _HEADERS_IMAGE_H_
#define _HEADERS_IMAGE_H_
#include <memory>
#include <string>
#include <tuple>
#include <vector>

enum class ImageOperations { SIMPLE_BLUR, GAUSSIAN_BLUR, SOBEL, GRAYSCALE };
class Image {
   public:
    std::vector<uint8_t> pixelData;
    int x_dim;
    int y_dim;
    int num_channels;
    std::string input_fname;
    std::string output_fname;
    ImageOperations op_type;

    Image();
    Image(const std::string ifname, const std::string ofname, ImageOperations op);
    Image(const uint8_t* original_data, int width, int height);
    ~Image();
};

#endif