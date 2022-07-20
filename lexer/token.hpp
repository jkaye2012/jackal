#pragma once

#include <cstddef>
#include <string_view>

namespace jackal::lexer
{
struct Token
{
  enum class Kind
  {
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
    // Signals a dyntax error
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

 private:
  Kind _kind;
  std::string_view _lexeme;
};
}  // namespace jackal::lexer