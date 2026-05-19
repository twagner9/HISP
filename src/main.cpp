#include "../headers/filters.hpp"
#include "../headers/image.hpp"
#include "CLI/App.hpp"
#include "CLI/CLI.hpp"
// #include "CLI/Config.hpp"
// #include "CLI/Formatter.hpp"
#include <iostream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, char **argv) {
    CLI::App app{
        "HISP, an abbreviation for Heterogenous Image & Signal Processing, is a"
        "CLI application that can apply image processing algorithms such as "
        "filters, FFTs, blurs, etc. to provided images in a high-performance "
        "manner. This means using multithreaded CPU when desired, as well as "
        "utilizing a GPU device if available.",
        "HISP"};

    // TODO: add flag for whether program will be on device or CPU

    std::string img_infname, img_ofname;
    app.add_option("Input image filename", img_infname, "The image to be used during program execution.");
    app.add_option("Output image filename", img_ofname,
                   "The name of the image that will be written after operation has been applied.");
    app.add_option("Image operation", static_cast<int>(ImageOperations::SIMPLE_BLUR),
                   "The operation to be performed on the input image.");

    bool use_mp{false};
    app.add_option("multiprocessing", use_mp, "Determine whether algorithms should execute using multiple threads.");
    try {
        CLI11_PARSE(app, argc, argv);
    } catch (CLI::ParseError &e) {
        std::cout << "Error parsing CLI inputs. " << e.what() << std::endl;
    }

    // TODO: handle user inputting different file extensions -- this should theoretically be okay, but the user should
    // be warned.

    std::cout << "Image filename is: " << img_infname << std::endl;
    std::cout << ((use_mp) ? "Using CPU-based multi-threading..." : "Using GPU device...") << std::endl;

    std::unique_ptr<Image> img = std::make_unique<Image>(img_infname, img_ofname);

    std::memcpy(img->data.data(), stbi_load(img->input_fname.c_str(), &img->x_dim, &img->y_dim, &img->num_channels, 0),
                img->x_dim * img->y_dim * img->num_channels * sizeof(unsigned char));

    if (img->op_type == ImageOperations::SIMPLE_BLUR) {
        std::cout << "Applying blur...\n\n";
        ImageFilters::blur(img.get(), 2);
        size_t ext_idx = img->input_fname.find_last_not_of('.');
        std::string ext = img->input_fname.substr(ext_idx);

        // TODO: determine whether allowing the user to save to a compatible image format that differs from input, then
        // implement the write operation using stb functions.

    } else {
        std::cout << "As of now, the simple blur operation is the only one implemented; check back from time to time "
                     "for new operations.\n";
    }
    return 0;
}