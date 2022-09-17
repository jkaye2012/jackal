#pragma once

#include <cstdint>

#include "util/source_location.hpp"

namespace jackal::lexer
{
/// @brief Tracks the current location within the source code.
///
/// The GPS is an implementation detail of the lexer that allows for each Token
/// to be easily tagged with its corresponding SourceLocation during the incremental
/// lexing process.
struct GPS
{
  /// @brief Constructs a GPS from the source code.
  ///
  /// @param line a pointer to the beginning of a source file
  explicit GPS(char const* line) noexcept;

  /// @brief Increments the source location's column by a specified number of characters.
  void column_moved(uint64_t chars) noexcept;

  /// @brief Increments the source location's line and resets the column position.
  void line_moved(char const* line) noexcept;

  /// @returns the current SourceLocation
  [[nodiscard]] util::SourceLocation current_location() const noexcept;

 private:
  util::Line _line;
  uint64_t _lineNum = 0;
  uint64_t _column = 0;
};
}  // namespace jackal::lexer
