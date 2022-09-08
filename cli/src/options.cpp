#include "cli/options.hpp"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <string>

#include <cxxopts.hpp>

#include "util/exit.hpp"

using jackal::cli::Options;

Options::Options(int argc, char** argv) noexcept
{
  cxxopts::Options options("jackal",
                           "Interface to the Jackal programming language and associated tooling\n");
  // clang-format off
  options.add_options()
    ("h,help", "Print usage")
    ("o,outputDir", "The compilation output directory", cxxopts::value<std::string>()->default_value(std::filesystem::current_path()))
    ("filePath", "The jackal source file to compile", cxxopts::value<std::string>());

  options.parse_positional({"filePath"});

  options.positional_help("SOURCE_FILE");
  // clang-format on

  try
  {
    auto result = options.parse(argc, argv);
    if (result.count("help") > 0)
    {
      std::cout << options.help() << std::endl;
      std::exit(0);
    }

    _outputDirectory = std::filesystem::path(result["outputDir"].as<std::string>());
    _filePath = result["filePath"].as<std::string>();
  }
  catch (cxxopts::OptionException const& ex)
  {
    std::cerr << "Failed to parse command line:" << std::endl;
    std::cerr << ex.what() << std::endl;
    std::exit(util::ExitInvalidArguments);
  }
}

auto Options::file_path() const noexcept -> std::string const& { return _filePath; }

auto Options::output_directory() const noexcept -> std::filesystem::path const&
{
  return _outputDirectory;
}
