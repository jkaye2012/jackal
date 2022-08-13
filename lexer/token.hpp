#pragma once

#include <cstddef>
#include <string>
#include <string_view>

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
    // [a-z][a-zA-Z\d]*
    Identifier,
    // =
    Equal,
    // +
    Plus,
    // End of program
    Halt,
    // Signals a syntax error
    Unknown
  };

  constexpr Token(Kind kind, char const* lexeme, std::size_t length) noexcept
      : _kind(kind), _lexeme(lexeme, length)
  {
  }

  constexpr Token(Kind kind, char const* lexemeBegin, char const* lexemeEnd) noexcept
      : _kind(kind), _lexeme(lexemeBegin, lexemeEnd)
  {
  }

  constexpr Kind kind() const noexcept { return _kind; }
  constexpr std::string_view lexeme() const noexcept { return _lexeme; }
  std::string lexeme_str() const noexcept { return std::string(_lexeme); }

 private:
  Kind _kind;
  std::string_view _lexeme;
};
}  // namespace jackal::lexer
