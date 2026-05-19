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

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main(int argc, char **argv) {
    CLI::App app{
        "HISP, an abbreviation for Heterogenous Image & Signal Processing, is a "
        "CLI application that can apply image processing algorithms such as "
        "filters, FFTs, blurs, etc. to provided images in a high-performance "
        "manner. This means using multithreaded CPU when desired, as well as "
        "utilizing a GPU device if available.",
        "HISP"};

    // TODO: add flag for whether program will be on device or CPU

    std::unordered_map<std::string, ImageOperations> op_map{{"blur", ImageOperations::SIMPLE_BLUR},
                                                            {"g_blur", ImageOperations::GAUSSIAN_BLUR},
                                                            {"seg", ImageOperations::SEGMENTATION},
                                                            {"gray", ImageOperations::GRAYSCALE}};

    // 1. Get all required inputs
    std::string img_ifname, img_ofname, op_string;
    ImageOperations selected_op{ImageOperations::SIMPLE_BLUR};
    app.add_option("-i, --input", img_ifname, "Input image to be used during program execution.")->check(CLI::ExistingFile)->required();

    app.add_option("-o, --output", img_ofname, "Output image that will be written after operation has been applied.")->required();

    app.add_option("-m, --mode", op_string, "The operation to be performed on the input image.")
        ->transform(CLI::CheckedTransformer(op_map, CLI::ignore_case))  // validate input against map keys
        ->required();

    bool use_mp{false};
    app.add_option("-p, --parallel", use_mp, "Whether algorithms should execute using multiple threads.");
    try {
        CLI11_PARSE(app, argc, argv);
    } catch (CLI::ParseError &e) {
        std::cout << "Error parsing CLI inputs. " << e.what() << std::endl;
    }

    // compare extensions and notify user if they're different
    // NOTE: this could be more robust; right now I trust the user and the files they're passing as being
    // the types specified, but they could theoretically not be, if the user changed the extension (.JPG to .PNG, e.g) incorrectly at some point.
    // Therefore verifying the file type through the bitmap would be the more pure way to do this.
    std::string ext;
    {
        std::string in_ext = img_ifname.substr(img_ifname.find_last_of('.') + 1, img_ifname.size());
        ext = img_ofname.substr(img_ofname.find_last_of(".") + 1, img_ofname.size());

        if (in_ext != ext) {
            std::cout << "\nNOTICE: the input filename has extension ." << in_ext << " while output has " << ext
                      << ". Saved image format will differ from input." << std::endl;
        }
    }

    // 2. For now, simply output what the user entered to ensure args are handled properly.
    std::cout << "Image input filename is: " << img_ifname << std::endl;
    std::cout << "Image output filename is: " << img_ofname << std::endl;
    std::cout << ((use_mp) ? "Using CPU-based multi-threading..." : "Using single thread...") << std::endl;
    std::cout << "Image operation: " << op_string << std::endl;
    ImageOperations user_selected_op = op_map[op_string];

    // 3. Create the Image object that will hold the required data, and copy the pixel data to it.
    std::unique_ptr<Image> img = std::make_unique<Image>(img_ifname, img_ofname);
    unsigned char *raw_data = stbi_load(img->input_fname.c_str(), &img->x_dim, &img->y_dim, &img->num_channels, 0);
    const int total_bytes = img->x_dim * img->y_dim * img->num_channels;
    img->data.resize(total_bytes);
    std::memcpy(img->data.data(), raw_data, total_bytes);
    free(raw_data);

    // TODO: Add basic profiling to view how long the operation takes -- this will come in handy when adding multithreading
    // 4. Apply user selected image operation.
    if (img->op_type == ImageOperations::SIMPLE_BLUR) {
        std::cout << "Applying blur...\n\n";
        ImageFilters::blur(img.get(), 10);
    } else if (img->op_type == ImageOperations::GAUSSIAN_BLUR) {
    } else if (img->op_type == ImageOperations::GRAYSCALE) {
    } else if (img->op_type == ImageOperations::SEGMENTATION) {
    } else {
        std::cerr << "Unknown image operation selected. Shouldn't get here. Abort.";
        free(raw_data);
    }

    // 5. Write out the modified image in the correct format.
    if (ext == "jpg") {
        stbi_write_jpg(img->output_fname.c_str(), img->x_dim, img->y_dim, img->num_channels, img->data.data(), 100);
    } else if (ext == "png") {
        stbi_write_png(img->output_fname.c_str(), img->x_dim, img->y_dim, img->num_channels, img->data.data(), img->x_dim * img->num_channels);
    } else {
        std::cerr << "Unrecognized output image format: " << ext << ". Aborting write." << std::endl;
        return 1;
    }

    std::cout << "Completed operation. Image written to " << img->output_fname << std::endl;

    return 0;
}