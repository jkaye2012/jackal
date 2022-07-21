#include "catch.hpp"
#include "lexer/lexer.hpp"
#include "lexer/token.hpp"

using jackal::lexer::Lexer;
using jackal::lexer::Token;

TEST_CASE("Equal should lex", "[lexer]")
{
  auto const* code = "=";
  Lexer lexer(code);
  Token result = lexer.next();

  REQUIRE(result.kind() == Token::Kind::Equal);
  REQUIRE(result.lexeme() == "=");
}

TEST_CASE("Plus should lex", "[lexer]")
{
  auto const* code = "+";
  Lexer lexer(code);
  Token result = lexer.next();

  REQUIRE(result.kind() == Token::Kind::Plus);
  REQUIRE(result.lexeme() == "+");
}

TEST_CASE("Single identifier should lex", "[lexer]")
{
  auto const* code = "foobar";
  Lexer lexer(code);
  Token result = lexer.next();

  REQUIRE(result.kind() == Token::Kind::Identifier);
  REQUIRE(result.lexeme() == "foobar");
}

TEST_CASE("Identifier can't start with uppercase", "[lexer]")
{
  auto const* code = "Foobar";
  Lexer lexer(code);
  Token result = lexer.next();

  REQUIRE(result.kind() == Token::Kind::Unknown);
  REQUIRE(result.lexeme() == "F");
}

TEST_CASE("Identifier can contain uppercase", "[lexer]")
{
  auto const* code = "fooBar";
  Lexer lexer(code);
  Token result = lexer.next();

  REQUIRE(result.kind() == Token::Kind::Identifier);
  REQUIRE(result.lexeme() == "fooBar");
}

TEST_CASE("Single integer should lex", "[lexer]")
{
  auto const* code = "12190";
  Lexer lexer(code);
  Token result = lexer.next();

  REQUIRE(result.kind() == Token::Kind::Number);
  REQUIRE(result.lexeme() == "12190");
}

TEST_CASE("Single floating-point value should lex", "[lexer]")
{
  auto const* code = "12190.123";
  Lexer lexer(code);
  Token result = lexer.next();

  REQUIRE(result.kind() == Token::Kind::Number);
  REQUIRE(result.lexeme() == "12190.123");
}

TEST_CASE("Invalid floating-point value should not lex", "[lexer]")
{
  auto const* code = "12190. ";
  Lexer lexer(code);
  Token result = lexer.next();

  REQUIRE(result.kind() == Token::Kind::Unknown);
  REQUIRE(result.lexeme() == "12190.");
}

TEST_CASE("Halt should lex", "[lexer]")
{
  auto const* code = "";
  Lexer lexer(code);
  Token result = lexer.next();

  REQUIRE(result.kind() == Token::Kind::Halt);
}
