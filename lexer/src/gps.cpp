#include "lexer/gps.hpp"

#include "util/source_location.hpp"

using jackal::lexer::GPS;

GPS::GPS(char const* line) noexcept : _line(line, 0) {}

auto GPS::column_moved(uint64_t chars) noexcept -> void { _column += chars; }

auto GPS::line_moved(char const* line) noexcept -> void
{
  _column = 0;
  _line = util::Line(line, ++_lineNum);
}

auto GPS::current_location() const noexcept -> util::SourceLocation
{
  return {_line, util::column(_column)};
}
