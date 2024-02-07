#include "main/render/linear_partition.h"
#include "main/scenes/scene.h"
#include "main/scenes/scene_settings.h"

#include <cstring>
#include <exception>
#include <format>
// clang-tidy doesn't recognize usage of this header
#include <fstream>
#include <getopt.h>//NOLINT
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <sstream>
#include <string>

auto asShortArguments(scene::RenderSettings const& settings) -> std::string
{
  return std::format("-r {}x{} -s {} -d {} -o {} -f {} -a {} -m {}",
    settings.imageWidth,
    settings.imageHeight,
    settings.sampleCount,
    settings.rayDepth,
    settings.outputFile,
    settings.degreesVerticalFOV,
    settings.defocusAngle,
    settings.focusDistance);
}

auto help() -> std::string
{
  return "ray_buster is a simple CPU based path tracing application, with a number of built in scenes.\n"
         "The tool will aim to utilize all available CPU cores.\n\n"
         "Usage: ray_buster --scene cornell-box [OPTIONS]\n"
         "Commands:\n"
         "\t--scene\t\tRender the selected scene with its built in default configuration.\n"
         "\t--list\t\tList all available scenes with their default settings displayed as ready to paste arguments for "
         "quick "
         "override.\n"
         "\t\t\tExample:\n"
         "\t\t\t\t-r 800x800 -s 100 -d 10 -o  -f 70 -a 0 -m 1\n"
         "\t-h --help\tPrint out the help message.\n\n"
         "For the scene command it is possible to override a number of optional the rendering settings, which are as "
         "follow:\n"
         "\t-r <width>x<height>\t- the rendering resolution. Example 1024x768\n"
         "\t-s <value>\t\t- sampling value. This controls how many sub pixel samples to take at each ray bounce.\n"
         "\t-d <value>\t\t- ray depth. How many ray bounces are simulated. After 20, it only gives diminishing "
         "returns.\n"
         "\t-o <value>\t\t- output file. The result will always use the PPM image format.\n"
         "\t-f <value>\t\t- vertical FOV in degrees.\n"
         "\t-a <value>\t\t- defocus angle. An angle for simulating camera focusing artifacts. A 0.0 disables the "
         "features.\n"
         "\t-m <value>\t\t- focus distance. The distance the camera is focusing at.\n\n"
         "Example usage:\n"
         "./ray_buster --scene cornell-box\n"
         "If the default configuration should be changed the easiest way is to list it with:\n"
         "./ray_buster --list\n"
         "Copy the appropriate parameter list and make the necessary modifications:\n"
         "./ray_buster --scene cornell-box -r 1000x1000 -s 1000 -d 20\n"
         "Parameters that we don't intend to override don't have to appear as arguments.\n";
}

auto list(std::map<std::string, scene::Configuration> const& scenes) -> std::string
{
  std::stringstream display;
  display << "List of available scenes:\n";
  for (auto const& entry : scenes) {
    display << std::format(
      "{:20}\t{}\n\t\t\t{}", entry.first, entry.second.description, asShortArguments(entry.second.settings))
            << '\n';
  }
  return display.str();
}

// Getopt man page: https://man7.org/linux/man-pages/man3/getopt.3.html
// NOLINTBEGIN(readability-function-cognitive-complexity)
auto main(int argc, char* argv[]) -> int
{
  try {
    auto selectedScene = std::string();
    auto imageWidth = std::optional<std::size_t>{};
    auto imageHeight = std::optional<std::size_t>{};
    auto sampleCount = std::optional<std::size_t>{};
    auto rayDepth = std::optional<std::size_t>{};
    auto outputFile = std::optional<std::string>{};
    auto degreesVerticalFOV = std::optional<double>{};
    auto defocusAngle = std::optional<double>{};
    auto focusDistance = std::optional<double>{};

    auto const resolutionRegex = std::regex{ R"((\d+)x(\d+))" };

    while (true) {
      auto optionIndex = 0;
      // option, optarg and getopt_long for some reason is not seen by the linter
      // NOLINTBEGIN(misc-include-cleaner)
      static auto const longOptions = std::array<struct option const, 4>({ { "scene", required_argument, nullptr, 0 },
        { "list", no_argument, nullptr, 0 },
        { "help", no_argument, nullptr, 0 },
        { nullptr, no_argument, nullptr, 0 } });

      auto charCode = getopt_long(argc, argv, "hr:s:d:o:f:a:m", longOptions.data(), &optionIndex);

      if (charCode == -1) { break; };
      switch (charCode) {
      case 0: {
        if (std::strncmp(longOptions.at(optionIndex).name, "help", sizeof("help")) == 0) {
          std::cout << help() << '\n';
          return 0;
        }
        if (std::strncmp(longOptions.at(optionIndex).name, "list", sizeof("list")) == 0) {
          std::cout << list(scene::configurations()) << '\n';
          return 0;
        }
        if (std::strncmp(longOptions.at(optionIndex).name, "scene", sizeof("scene")) == 0) {
          selectedScene = std::string(optarg);
        }
        break;
      }
      case 'h': {
        std::cout << help() << '\n';
        return 0;
      }
      case 'r': {
        auto resolutionText = std::string{ optarg };
        auto baseMatch = std::smatch{};
        if (!std::regex_match(resolutionText, baseMatch, resolutionRegex)) {
          std::cerr << std::format(
            "Invalid resolution argument received. Expected: '<width>x<height>, Got: {}'", resolutionText);
          return 1;
        }
        try {
          imageWidth = std::stoull(baseMatch[1].str());
          imageHeight = std::stoull(baseMatch[2].str());
        } catch (std::exception const& e) {
          std::cerr << std::format("Failed to parse '-r' argument. Reason: {}", e.what()) << '\n';
          return 1;
        }
        break;
      }
      case 's': {
        auto sampleTest = std::string{ optarg };
        try {
          sampleCount = std::stoull(sampleTest);
        } catch (std::exception const& e) {
          std::cerr << std::format("Failed to parse '-s' argument. Reason: {}", e.what()) << '\n';
          return 1;
        }
        break;
      }
      case 'd': {
        auto rayDepthText = std::string{ optarg };
        try {
          rayDepth = std::stoull(rayDepthText);
        } catch (std::exception const& e) {
          std::cerr << std::format("Failed to parse '-d' argument. Reason: {}", e.what()) << '\n';
          return 1;
        }
        break;
      }
      case 'o': {
        outputFile = std::string{ optarg };
        break;
      }
      case 'f': {
        auto degreesVerticalFOVText = std::string{ optarg };
        try {
          degreesVerticalFOV = std::stod(degreesVerticalFOVText);
        } catch (std::exception const& e) {
          std::cerr << std::format("Failed to parse '-f' argument. Reason: {}", e.what()) << '\n';
          return 1;
        }
        break;
      }
      case 'a': {
        auto defocusAngleText = std::string{ optarg };
        try {
          defocusAngle = std::stod(defocusAngleText);
        } catch (std::exception const& e) {
          std::cerr << std::format("Failed to parse '-a' argument. Reason: {}", e.what()) << '\n';
          return 1;
        }
        break;
      }
      case 'm': {
        auto focusDistanceText = std::string{ optarg };
        try {
          focusDistance = std::stod(focusDistanceText);
        } catch (std::exception const& e) {
          std::cerr << std::format("Failed to parse '-m' argument. Reason: {}", e.what()) << '\n';
          return 1;
        }
        break;
      }
      case '?': {
        break;
      }
      default: {
        std::cout << "Unhandled character code received: " << charCode << '\n';
      }
      }
      // NOLINTEND(misc-include-cleaner)
    }

    auto const configs = scene::configurations();
    auto selected = configs.find(selectedScene);
    if (selected == configs.end()) {
      std::cerr << std::format("Missing configuration for scene: {}", selectedScene) << '\n';
      return 1;
    }

    auto consolidatedSettings = selected->second.settings;
    if (imageWidth && imageHeight) {
      consolidatedSettings.imageWidth = imageWidth.value();
      consolidatedSettings.imageHeight = imageHeight.value();
    }
    if (sampleCount) { consolidatedSettings.sampleCount = sampleCount.value(); }
    if (rayDepth) { consolidatedSettings.rayDepth = rayDepth.value(); }
    if (outputFile) { consolidatedSettings.outputFile = outputFile.value(); }
    if (degreesVerticalFOV) { consolidatedSettings.degreesVerticalFOV = degreesVerticalFOV.value(); }
    if (defocusAngle) { consolidatedSettings.defocusAngle = defocusAngle.value(); }
    if (focusDistance) { consolidatedSettings.focusDistance = focusDistance.value(); }

    auto renderResult = std::ofstream{ consolidatedSettings.outputFile, std::ios_base::out | std::ios_base::trunc };
    if (!renderResult.is_open()) {
      std::cerr << std::format("Failed to open file: '{}'", consolidatedSettings.outputFile);
      return 1;
    }
    render::linearPartition(selected->second.sceneLoader(consolidatedSettings), renderResult);
  } catch (std::exception const& e) {
    std::cerr << std::format("Unhandled exception:\n{}", e.what()) << '\n';
    return 1;
  }

  return 0;
}
// NOLINTEND(readability-function-cognitive-complexity)