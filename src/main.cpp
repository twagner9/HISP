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
        "HISP, an abbreviation for Heterogenous Image & Signal Processing, is "
        "a "
        "CLI application that can apply image processing algorithms such as "
        "filters, FFTs, blurs, etc. to provided images in a high-performance "
        "manner. This means using multithreaded CPU when desired, as well as "
        "utilizing a GPU device if available.",
        "HISP"};

    // TODO: add flag for whether program will be on device or CPU

    std::string img_fname;
    app.add_option("Image filename", img_fname,
                   "The image to be used during program execution.");

    int img_x_dim{0}, img_y_dim{0}, img_channels{1};
    app.add_option("Image X dim", img_x_dim,
                   "The width of the image being passed.");
    app.add_option("Image Y dim", img_y_dim,
                   "The height of the image being passed.");
    app.add_option("Number of image channels", img_channels,
                   "The number of channels the image has.");

    bool use_mp{false};
    app.add_option(
        "multiprocessing", use_mp,
        "Determine whether algorithms should execute using multiple threads.");
    try {
        CLI11_PARSE(app, argc, argv);
    } catch (CLI::ParseError &e) {
        std::cout << "Error parsing CLI inputs. " << e.what() << std::endl;
    }

    std::cout << "Image filename is: " << img_fname << std::endl;
    std::cout << ((use_mp) ? "Using CPU-based multi-threading..."
                           : "Using GPU device...")
              << std::endl;

    std::unique_ptr<Image> img = std::make_unique<Image>();

    std::memcpy(
        img->data.data(),
        stbi_load(img_fname.c_str(), &img->x_dim, &img->y_dim,
                  &img->num_channels, 0),
        img->x_dim * img->y_dim * img->num_channels * sizeof(unsigned char));

    return 0;
}