#pragma once

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <optional>
#include <ostream>
#include <random>
#include <string>

#include "util/exit.hpp"

namespace jackal::util
{
/// @brief Attempts to read a file's content from the filesystem.
///
/// The file is read eagerly in its entirety without buffering.
///
/// @returns the file's contents if it could be read
/// @returns std::nullopt if it could not be read for any reason.
inline std::optional<std::string> read_file(std::filesystem::path const& path) noexcept
{
  std::string file;
  try
  {
    std::ifstream inputStream(path);
    std::stringstream iss;
    iss << inputStream.rdbuf();
    return iss.str();
  }
  catch (std::ios_base::failure const& fail)
  {
    return std::nullopt;
  }
}

/// @brief Creates a temporary directory on disk that can be used for short-lived file storage.
///
/// The created directory is not automatically cleaned up or managed in any way after the function
/// returns.
///
/// While this function cannot throw exceptions, it will terminate the running process if a
/// temporary directory cannot be created.
///
/// @returns the path that was uniquely created for use by the running process
/// @see TemporaryDirectory for an RAII wrapper around this functionality that should be used in
/// most situations
inline std::filesystem::path temp_dir() noexcept
{
  static constexpr auto MAX_ATTEMPTS = 1000;
  auto tmp_root = std::filesystem::temp_directory_path();
  std::random_device dev;
  std::mt19937 prng(dev());
  std::uniform_int_distribution<uint64_t> rand(0);

  std::filesystem::path path;
  for (auto i = 0; i < MAX_ATTEMPTS; ++i)
  {
    std::stringstream ss;
    ss << std::hex << rand(prng);
    path = tmp_root / ss.str();
    if (std::filesystem::create_directory(path))
    {
      return path;
    }
  }

  std::cerr << "Could not create temporary directory" << std::endl;
  std::exit(ExitCouldNotCreateTempDir);
}

/// @brief An RAII wrapper for an ephemeral directory on disk.
struct TemporaryDirectory
{
  /// @brief Creates a temporary directory.
  TemporaryDirectory() noexcept : _directory(temp_dir()) {}

  /// @brief Removes the created temporary directory and all contents contained within.
  ~TemporaryDirectory() noexcept { std::filesystem::remove_all(_directory); }

  TemporaryDirectory(TemporaryDirectory const&) = delete;
  TemporaryDirectory& operator=(TemporaryDirectory const&) = delete;
  TemporaryDirectory(TemporaryDirectory&&) noexcept = default;
  TemporaryDirectory& operator=(TemporaryDirectory&&) noexcept = default;

  /// @returns the ephemeral temporary directory.
  [[nodiscard]] std::filesystem::path const& directory() const noexcept { return _directory; }

 private:
  std::filesystem::path _directory;
};

}  // namespace jackal::util
