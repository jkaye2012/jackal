#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string_view>
#include <utility>

namespace jackal::tests
{
/// @brief The path for static test resource files.
static const std::filesystem::path TestResourcePath("./tests/resources");

/// @brief A static file read from disk for testing purposes.
struct FileTestResource
{
  /// @brief Constructs a new file resource from a fully-qualified file name.
  ///
  /// @param fileName the name of the file to read, including its extension
  explicit FileTestResource(std::string fileName) noexcept : _fileName(std::move(fileName))
  {
    std::ifstream inputStream(TestResourcePath / _fileName);
    std::stringstream iss;
    iss << inputStream.rdbuf();
    _content = iss.str();
  }

  /// @returns the fully-qualified name of the file resource
  [[nodiscard]] std::string_view file_name() const noexcept { return _fileName; }

  /// @returns the on-disk content of the file resource
  [[nodiscard]] std::string_view content() const noexcept { return _content; }

  /// @returns the C string representation of the file resource
  [[nodiscard]] char const* data() const noexcept { return _content.data(); }

 private:
  std::string _fileName;
  std::string _content;
};
}  // namespace jackal::tests
