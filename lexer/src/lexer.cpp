#include "lexer/lexer.hpp"

#include <cctype>
#include <cstddef>

#include "lexer/token.hpp"

using jackal::lexer::Lexer;

auto Lexer::peek() const noexcept -> char { return *_code; }

auto Lexer::peek_n(std::size_t n) const noexcept -> char { return *(_code + n); }

auto Lexer::get() noexcept -> char const* { return _code++; }

auto Lexer::tok_unary(Token::Kind kind) noexcept -> Token { return {kind, get(), 1}; }

auto Lexer::tok_number() noexcept -> Token
{
  char const* lexemeBegin = get();
  while (isdigit(peek()) != 0)
  {
    get();
  }
  if (peek() == '.')
  {
    if (isdigit(peek_n(2)) == 0)
    {
      get();
      return {Token::Kind::Unknown, lexemeBegin, _code};
    }

    get();
    get();
  }
  while (isdigit(peek()) != 0)
  {
    get();
  }
  char const* lexemeEnd = _code;

  return {Token::Kind::Number, lexemeBegin, lexemeEnd};
}

auto Lexer::tok_identifier() noexcept -> Token
{
  char const* lexemeBegin = get();
  while (isalpha(peek()) != 0)
  {
    get();
  }
  char const* lexemeEnd = _code;

  return {Token::Kind::Identifier, lexemeBegin, lexemeEnd};
}

auto Lexer::next() noexcept -> Token
{
  if (!_peek.empty())
  {
    auto token = _peek.front();
    _peek.pop_front();
    return token;
  }

  return _next();
}

auto Lexer::_next() noexcept -> Token
{
  while (peek() == ' ')
  {
    get();
  }

  char const current = peek();
  if (current == '\0')
  {
    return {Token::Kind::Halt, _code, 1};
  }
  if (current == '\n')
  {
    return tok_unary(Token::Kind::Newline);
  }
  if (current == '+')
  {
    return tok_unary(Token::Kind::Plus);
  }
  if (current == '=')
  {
    return tok_unary(Token::Kind::Equal);
  }
  if (isdigit(current) != 0)
  {
    return tok_number();
  }
  if (isalpha(current) != 0 && islower(current) != 0)
  {
    return tok_identifier();
  }

  return tok_unary(Token::Kind::Unknown);
}

auto Lexer::is_halted() noexcept -> bool { return _peek.empty() && peek() == '\0'; }
