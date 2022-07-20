#include "lexer/lexer.hpp"

#include <cctype>
#include <cstddef>

#include "lexer/token.hpp"

using jackal::lexer::Lexer;

constexpr char const Lexer::peek() const noexcept { return *_code; }

constexpr char const Lexer::peek_n(std::size_t n) const noexcept { return *(_code + n); }

constexpr char const* Lexer::get() noexcept { return _code++; }

constexpr auto Lexer::tok_unary(Token::Kind kind) noexcept -> Token { return {kind, get(), 1}; }

constexpr auto Lexer::tok_number() noexcept -> Token
{
  char const* lexemeBegin = get();
  while (isdigit(peek()))
  {
    get();
  }
  if (peek() == '.' && isdigit(peek_n(2)))
  {
    get();
    get();
  }
  while (isdigit(peek()))
  {
    get();
  }
  char const* lexemeEnd = _code;

  return {Token::Kind::Number, lexemeBegin, lexemeEnd};
}

constexpr auto Lexer::tok_identifier() noexcept -> Token
{
  char const* lexemeBegin = get();
  while (isalpha(peek()))
  {
    get();
  }
  char const* lexemeEnd = _code;

  return {Token::Kind::Identifier, lexemeBegin, lexemeEnd};
}

constexpr auto Lexer::next() noexcept -> Token
{
  while (std::isspace(peek()))
  {
    get();
  }

  char const current = peek();
  if (current == '\0')
  {
    return tok_unary(Token::Kind::Halt);
  }
  else if (current == '+')
  {
    return tok_unary(Token::Kind::Plus);
  }
  else if (current == '=')
  {
    return tok_unary(Token::Kind::Equal);
  }
  else if (isdigit(current))
  {
    return tok_number();
  }
  else if (isalpha(current) && islower(current))
  {
    return tok_identifier();
  }
  else
  {
    return tok_unary(Token::Kind::Unknown);
  }
}
