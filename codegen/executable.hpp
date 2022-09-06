#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

#include "util/file_system.hpp"

namespace jackal::codegen
{
/// @brief The result of compiling an executable Jackal source file.
///
/// Provides introspection capabilities on the high-level compilation outputs, and allows
/// the executable to be run and validated (primarily for internal testing purposes).
struct Executable
{
  /// @brief Creates an Executable that will use a randomly generated temporary directory.
  Executable(std::string name, std::string source) noexcept;
  /// @brief Creates an Executable that will use the provided temporary directory.
  Executable(std::string name, std::string source, util::TemporaryDirectory&& directory) noexcept;

  /// @brief Attempts to compile the provided intermediate source code to an on-disk executable.
  ///
  /// If compilation has already succeeded previously, this function will not re-compile. This means
  /// that it is possible for this function to return a path that does not point to an on-disk file
  /// (if an external process has removed the file after it was originally compiled).
  ///
  /// @returns std::nullopt if compilation fails
  /// @returns the path to the executable file if compilation succeeds
  [[nodiscard]] std::optional<std::string_view> compile() noexcept;

  /// @brief Attempts to execute the Executable in an external shell.
  ///
  /// This function will attempt to compile the executable if it has not already been compiled.
  ///
  /// @see jackal::util::exec
  [[nodiscard]] std::optional<std::string> execute() noexcept;

  /// @returns the name of the executable as defined by the Jackal specification
  [[nodiscard]] std::string name() const noexcept { return _name; }

  /// @returns the generated intermediate source code of the executable
  [[nodiscard]] std::string source() const noexcept { return _source; }

 private:
  std::string _name;
  std::string _source;
  util::TemporaryDirectory _dir;
  std::optional<std::string> _path;
};
}  // namespace jackal::codegen
