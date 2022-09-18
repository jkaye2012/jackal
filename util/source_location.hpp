#pragma once

#include <cstdint>
#include <sstream>
#include <string>
#include <string_view>

#include "util/phantom.hpp"

namespace jackal::util
{
// clang-format off
struct ColumnTag {};
// clang-format on

using Column = Phantom<ColumnTag, uint64_t>;
[[nodiscard]] constexpr Column column(uint64_t value) noexcept { return {value}; }

/// @brief A single line of the Jackal source code being parsed by the compiler.
///
/// Working with @p Line allows for greatly simplified error messaging and diagnostics
/// throughout the compiler infrastructure.
struct Line
{
  /// @brief Constructs a Line from a location within the source code.
  ///
  /// @param line a pointer to the beginning of the line
  /// @param lineNum the 0-based index to the number of the line within the source file
  constexpr Line(char const* line, uint64_t lineNum) noexcept : _num(lineNum)
  {
    std::string_view rawLine(line);
    _src = rawLine.substr(0, rawLine.find('\n'));
  }

  /// @returns the line of source code
  [[nodiscard]] constexpr std::string_view src() const noexcept { return _src; }

  /// @returns the line number
  [[nodiscard]] constexpr uint64_t num() const noexcept { return _num; }

 private:
  std::string_view _src;
  uint64_t _num;
};

/// @brief A single point within the Jackal source code being parsed by the compiler.
///
/// Each @p SourceLocation corresponds to exactly one lexed Token within the source.
struct SourceLocation
{
  constexpr SourceLocation(Line line, Column column) noexcept : _line(line), _column(column.value)
  {
  }

  [[nodiscard]] constexpr Line line() const noexcept { return _line; }

  [[nodiscard]] constexpr uint64_t column() const noexcept { return _column; }

 private:
  Line _line;
  uint64_t _column;
};

/// @brief Converts a SourceLocation to a string representation useful for diagnostic messaging.
///
/// The string returned does not contain a trailing newline. It is meant to be used as a prefix
/// for a message to be displayed to the user (usually for construction of an error message).
///
/// @param loc the source location to process
/// @returns a human readable string that should be appended with a message
[[nodiscard]] inline std::string to_string(SourceLocation const& loc) noexcept
{
  std::ostringstream oss;
  oss << loc.line().src() << std::endl;
  for (auto i = 0; i < loc.column(); ++i)
  {
    oss << ' ';
  }
  oss << '^' << std::endl;
  for (auto i = 0; i < loc.column(); ++i)
  {
    oss << ' ';
  }
  oss << "└----";
  return oss.str();
}
}  // namespace jackal::util
