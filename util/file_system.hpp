#pragma once

#include <exception>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <random>

namespace jackal::util
{
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

  std::cerr << "Terminating: could not create temporary directory" << std::endl;
  std::terminate();
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
