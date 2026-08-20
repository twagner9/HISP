#include <chrono>
#include <iostream>
#include <string>

#include "../headers/filters.hpp"
#include "../headers/image.hpp"
#include "CLI/App.hpp"
#include "CLI/CLI.hpp"

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
                                                            {"seg", ImageOperations::SOBEL},
                                                            {"gray", ImageOperations::GRAYSCALE}};

    // 1. Get all required inputs
    std::string img_ifname, img_ofname, op_string;
    ImageOperations selected_op{ImageOperations::SIMPLE_BLUR};
    app.add_option("-i, --input", img_ifname, "Input image to be used during program execution.")->check(CLI::ExistingFile)->required();
    app.add_option("-o, --output", img_ofname, "Output image that will be written after operation has been applied.")->required();
    auto mode_opt = app.add_option("-m, --mode", selected_op, "The operation to be performed on the input image.")
                        ->transform(CLI::CheckedTransformer(op_map, CLI::ignore_case))  // validate input against map keys
                        ->required();

    int user_blur_rad{1};
    auto blur_opt = app.add_option("-r, --radius", user_blur_rad, "The pixel radius to be used when averaging pixels for the blur operation.");

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

    // 3. Create the Image object that will hold the required data, and copy the pixel data to it.
    std::unique_ptr<Image> img = std::make_unique<Image>(img_ifname, img_ofname, selected_op);
    unsigned char *raw_data = stbi_load(img->input_fname.c_str(), &img->x_dim, &img->y_dim, &img->num_channels, 0);
    const int total_bytes{img->x_dim * img->y_dim * img->num_channels};

    img->pixelData.resize(total_bytes);
    std::memcpy(img->pixelData.data(), raw_data, total_bytes);
    free(raw_data);

    if (user_blur_rad <= 0) {
        std::cout << "Must set blur radius to be greater than 0; defaulting to radius of 1." << std::endl;
        user_blur_rad = 1;
    }
    // TODO: Add basic profiling to view how long the operation takes -- this will come in handy when adding multithreading
    // 4. Apply user selected image operation.
    auto time_start{std::chrono::high_resolution_clock::now()};
    if (img->op_type == ImageOperations::SIMPLE_BLUR) {
        std::cout << "\nApplying blur..." << std::endl;
        ImageFilters::blur(img.get(), user_blur_rad);
    } else if (img->op_type == ImageOperations::GAUSSIAN_BLUR) {
        std::cout << "\nApplying Gaussian Blur..." << std::endl;
        ImageFilters::gaussian_blur(img.get(), user_blur_rad);
    } else if (img->op_type == ImageOperations::GRAYSCALE) {
        std::cout << "\nConverting to grayscale..." << std::endl;
        ImageFilters::grayscale(img.get());
    } else if (img->op_type == ImageOperations::SOBEL) {
    } else {
        std::cerr << "Unknown image operation selected. Shouldn't get here. Abort.";
        return 1;
    }

    // A bit of profiling to see how long the operations are taking.
    auto time_end{std::chrono::high_resolution_clock::now()};
    auto duration{time_end - time_start};
    auto minutes{std::chrono::duration_cast<std::chrono::minutes>(duration)};
    auto seconds{std::chrono::duration_cast<std::chrono::seconds>(duration - minutes)};

    std::cout << std::setfill('0') << "\nJob runtime:\t\t" << std::setw(2) << minutes.count() << ":" << std::setw(2) << seconds.count() << std::endl;
    std::cout << std::endl;

    // 5. Write out the modified image in the correct format.
    if (ext == "jpg") {
        stbi_write_jpg(img->output_fname.c_str(), img->x_dim, img->y_dim, img->num_channels, img->pixelData.data(), 100);
    } else if (ext == "png") {
        stbi_write_png(img->output_fname.c_str(), img->x_dim, img->y_dim, img->num_channels, img->pixelData.data(), img->x_dim * img->num_channels);
    } else {
        std::cerr << "Unrecognized output image format: " << ext << ". Aborting write." << std::endl;
        return 1;
    }

    std::cout << "Completed operation. Image written to " << img->output_fname << std::endl;

    return 0;
}