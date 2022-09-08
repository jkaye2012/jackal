#pragma once

#include <filesystem>
#include <string>

namespace jackal::cli
{
struct Options
{
  Options(int argc, char** argv) noexcept;

  [[nodiscard]] std::string const& file_path() const noexcept;

  [[nodiscard]] std::filesystem::path const& output_directory() const noexcept;

 private:
  std::string _filePath;
  std::filesystem::path _outputDirectory;
};
}  // namespace jackal::cli
