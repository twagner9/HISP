#include "CLI/App.hpp"
#include "CLI/CLI.hpp"
// #include "CLI/Config.hpp"
// #include "CLI/Formatter.hpp"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  CLI::App app{
      "HISP, an abbreviation for Heterogenous Image & Signal Processing, is a "
      "CLI application that can apply image processing algorithms such as "
      "filters, FFTs, blurs, etc. to provided images in a high-performance "
      "manner. This means using multithreaded CPU when desired, as well as "
      "utilizing a GPU device if available.",
      "HISP"};

  // TODO: add flag for whether program will be on device or CPU

  std::string default_message;
  app.add_option(
      "message", default_message,
      "The default printed message to be displayed on program execution.");

  bool use_mp{false};
  app.add_option(
      "multiprocessing", use_mp,
      "Determine whether algorithms should execute using multiple threads.");
  try {
    CLI11_PARSE(app, argc, argv);
  } catch (CLI::ParseError &e) {
    std::cout << "Error parsing CLI inputs. " << e.what() << std::endl;
  }

  std::cout << "Message is: " << default_message << std::endl;
  std::cout << ((use_mp) ? "Using CPU-based multi-threading..."
                         : "Using GPU device...")
            << std::endl;
  return 0;
}