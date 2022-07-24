#include <catch.hpp>
#include <string_view>

#include "lexer/lexer.hpp"
#include "lexer/token.hpp"

using jackal::lexer::Lexer;
using jackal::lexer::Token;

static void require_next(Lexer& lexer, Token::Kind kind, std::string_view lexeme)
{
  Token result = lexer.next();
  REQUIRE(result.kind() == kind);
  if (!lexeme.empty())
  {
    REQUIRE(result.lexeme() == lexeme);
  }
}

static void require_next(Lexer& lexer, Token::Kind kind) { require_next(lexer, kind, ""); }

TEST_CASE("Newline should lex", "[lexer][lexer_token]")
{
  auto const* code = "\n";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Newline, "\n");
}

TEST_CASE("Equal should lex", "[lexer][lexer_token]")
{
  auto const* code = "=";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Equal, "=");
}

TEST_CASE("Plus should lex", "[lexer][lexer_token]")
{
  auto const* code = "+";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Plus, "+");
}

TEST_CASE("Single identifier should lex", "[lexer][lexer_token]")
{
  auto const* code = "foobar";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Identifier, "foobar");
}

TEST_CASE("Identifier starting with uppercase should lex", "[lexer][lexer_token]")
{
  auto const* code = "Foobar";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Unknown, "F");
}

TEST_CASE("Identifier containing uppercase should lex", "[lexer][lexer_token]")
{
  auto const* code = "fooBar";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Identifier, "fooBar");
}

TEST_CASE("Single integer should lex", "[lexer][lexer_token]")
{
  auto const* code = "12190";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Number, "12190");
}

TEST_CASE("Single floating-point value should lex", "[lexer][lexer_token]")
{
  auto const* code = "12190.123";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Number, "12190.123");
}

TEST_CASE("Invalid floating-point value should not lex", "[lexer][lexer_token]")
{
  auto const* code = "12190. ";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Unknown, "12190.");
}

TEST_CASE("Halt should lex", "[lexer][lexer_token]")
{
  auto const* code = "";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Halt);
}

TEST_CASE("Constant assignment expression should lex", "[lexer][lexer_expression]")
{
  auto const* code = "let x = 12";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Identifier, "let");
  require_next(lexer, Token::Kind::Identifier, "x");
  require_next(lexer, Token::Kind::Equal, "=");
  require_next(lexer, Token::Kind::Number, "12");
  require_next(lexer, Token::Kind::Halt);
}

TEST_CASE("Compound assignment expression should lex", "[lexer][lexer_expression]")
{
  auto const* code = "let x = y + z";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Identifier, "let");
  require_next(lexer, Token::Kind::Identifier, "x");
  require_next(lexer, Token::Kind::Equal, "=");
  require_next(lexer, Token::Kind::Identifier, "y");
  require_next(lexer, Token::Kind::Plus, "+");
  require_next(lexer, Token::Kind::Identifier, "z");
  require_next(lexer, Token::Kind::Halt);
}

TEST_CASE("Print expression should lex", "[lexer][lexer_expression]")
{
  auto const* code = "print 123";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Identifier, "print");
  require_next(lexer, Token::Kind::Number, "123");
  require_next(lexer, Token::Kind::Halt);
}
