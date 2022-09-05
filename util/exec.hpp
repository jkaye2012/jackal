#pragma once

#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace jackal::util
{
/// @brief Attempts to execute a shell command, returning its stdout if successful.
///
/// @returns std::nullopt if the shell command fails (or the shell cannot be opened)
/// @returns The stdout pipe of the executed shell as a string if the command succeeds
std::optional<std::string> exec(std::string_view command) noexcept
{
  std::array<char, 128> buffer;  // NOLINT
  std::ostringstream oss;

  auto *pipe = popen(command.data(), "r");
  if (pipe == nullptr)
  {
    return std::nullopt;
  }

  while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
  {
    oss << buffer.data();
  }

  auto returnCode = pclose(pipe);
  if (returnCode != 0)
  {
    return std::nullopt;
  }

  return oss.str();
}
}  // namespace jackal::util
