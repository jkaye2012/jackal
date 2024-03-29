#pragma once

#include <cstddef>
#include <string>
#include <string_view>

#include "util/source_location.hpp"

namespace jackal::lexer
{
struct Token
{
  enum class Kind
  {
    // \n
    Newline,
    // \d+[\.\d+]?
    Number,
    // '.'
    Char,
    // ".*"
    String,
    // true|false
    Boolean,
    // [a-z]+
    Keyword,
    // [a-z][a-z_\d]*
    ValueIdentifier,
    // [A-Z][a-zA-Z\d]*
    TypeIdentifier,
    // ::
    Is,
    // ->
    Returns,
    // ;
    End,
    // =
    Equal,
    // .
    Dot,
    // ,
    Comma,
    // {
    OpenScope,
    // }
    CloseScope,
    // [
    OpenContext,
    // ]
    CloseContext,
    // (
    OpenGroup,
    // )
    CloseGroup,
    // End of program
    Halt,
    // Signals a syntax error
    Unknown
  };

  constexpr Token(Kind kind, util::SourceLocation location, char const* lexeme,
                  std::size_t length) noexcept
      : _kind(kind), _location(location), _lexeme(lexeme, length)
  {
  }

  constexpr Token(Kind kind, util::SourceLocation location, char const* lexemeBegin,
                  char const* lexemeEnd) noexcept
      : _kind(kind), _location(location), _lexeme(lexemeBegin, lexemeEnd)
  {
  }

  [[nodiscard]] constexpr Kind kind() const noexcept { return _kind; }

  [[nodiscard]] constexpr util::SourceLocation location() const noexcept { return _location; }

  [[nodiscard]] constexpr std::string_view lexeme() const noexcept { return _lexeme; }

  [[nodiscard]] std::string lexeme_str() const noexcept { return std::string(_lexeme); }

  auto operator<=>(Token const&) const noexcept = default;

 private:
  Kind _kind;
  util::SourceLocation _location;
  std::string_view _lexeme;
};
}  // namespace jackal::lexer
