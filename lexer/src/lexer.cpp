#include "lexer/lexer.hpp"

#include <cctype>
#include <cstddef>

#include "lexer/token.hpp"

using jackal::lexer::Lexer;

char const Lexer::peek() const noexcept { return *_code; }

char const Lexer::peek_n(std::size_t n) const noexcept { return *(_code + n); }

char const* Lexer::get() noexcept { return _code++; }

auto Lexer::tok_unary(Token::Kind kind) noexcept -> Token { return {kind, get(), 1}; }

auto Lexer::tok_number() noexcept -> Token
{
  char const* lexemeBegin = get();
  while (isdigit(peek()))
  {
    get();
  }
  if (peek() == '.')
  {
    if (!isdigit(peek_n(2)))
    {
      get();
      return {Token::Kind::Unknown, lexemeBegin, _code};
    }

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

auto Lexer::tok_identifier() noexcept -> Token
{
  char const* lexemeBegin = get();
  while (isalpha(peek()))
  {
    get();
  }
  char const* lexemeEnd = _code;

  return {Token::Kind::Identifier, lexemeBegin, lexemeEnd};
}

auto Lexer::next() noexcept -> Token
{
  while (peek() == ' ')
  {
    get();
  }

  char const current = peek();
  if (current == '\0')
  {
    return tok_unary(Token::Kind::Halt);
  }
  else if (current == '\n')
  {
    return tok_unary(Token::Kind::Newline);
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
