#include "tests/catch.hpp"

#include <array>
#include <cstddef>
#include <initializer_list>
#include <string_view>
#include <utility>

#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include "tests/resource.hpp"
#include "util/source_location.hpp"

using jackal::lexer::Lexer;
using jackal::lexer::Token;

static void require_next(Lexer& lexer, Token::Kind kind, std::string_view lexeme)
{
  Token result = lexer.next();
  if (!lexeme.empty())
  {
    REQUIRE(result.lexeme() == lexeme);
  }
  REQUIRE(result.kind() == kind);
}

static void require_next(Lexer& lexer, Token::Kind kind) { require_next(lexer, kind, ""); }

static void require_sequence(
    Lexer& lexer, std::initializer_list<std::pair<Token::Kind, std::string_view>> const& tokens)
{
  for (auto const& [kind, lexeme] : tokens)
  {
    require_next(lexer, kind, lexeme);
  }
  require_next(lexer, Token::Kind::Halt);
}

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

TEST_CASE("Char should lex", "[lexer][lexer_token]")
{
  auto const* code = "'x'";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Char, "'x'");
}

TEST_CASE("String should lex", "[lexer][lexer_token]")
{
  auto const* code = "\"this is a pretty cool string\"";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::String, "\"this is a pretty cool string\"");
}

TEST_CASE("Multi-line string should lex", "[lexer][lexer_token]")
{
  auto const* code = "\"this string\nspans multiple\nlines\"";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::String, "\"this string\nspans multiple\nlines\"");
}

TEST_CASE("Boolean with true value should lex", "[lexer][lexer_token]")
{
  auto const* code = "true";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Boolean, "true");
}

TEST_CASE("Boolean with false value should lex", "[lexer][lexer_token]")
{
  auto const* code = "false";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Boolean, "false");
}

TEST_CASE("Halt should lex", "[lexer][lexer_token]")
{
  auto const* code = "";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Halt);
}

TEST_CASE("Keywords should lex", "[lexer][lexer_token]")
{
  auto const* code = "let fn data concept satisfy effect interpret";
  Lexer lexer(code);

  require_sequence(lexer, {
                              {Token::Kind::Keyword, "let"},
                              {Token::Kind::Keyword, "fn"},
                              {Token::Kind::Keyword, "data"},
                              {Token::Kind::Keyword, "concept"},
                              {Token::Kind::Keyword, "satisfy"},
                              {Token::Kind::Keyword, "effect"},
                              {Token::Kind::Keyword, "interpret"},
                          });
}

TEST_CASE("Value identifiers should lex", "[lexer][lexer_token]")
{
  auto const* code = "xyz ab_cd ab123 jk_12190_ck";
  Lexer lexer(code);

  require_sequence(lexer, {
                              {Token::Kind::ValueIdentifier, "xyz"},
                              {Token::Kind::ValueIdentifier, "ab_cd"},
                              {Token::Kind::ValueIdentifier, "ab123"},
                              {Token::Kind::ValueIdentifier, "jk_12190_ck"},
                          });
}

TEST_CASE("Type identifiers should lex", "[lexer][lexer_token]")
{
  auto const* code = "Type List MultiType Sha256";
  Lexer lexer(code);

  require_sequence(lexer, {
                              {Token::Kind::TypeIdentifier, "Type"},
                              {Token::Kind::TypeIdentifier, "List"},
                              {Token::Kind::TypeIdentifier, "MultiType"},
                              {Token::Kind::TypeIdentifier, "Sha256"},
                          });
}

TEST_CASE("Is should lex", "[lexer][lexer_token]")
{
  auto const* code = "::";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Is, "::");
}

TEST_CASE("Is with extra colon should not lex", "[lexer][lexer_token]")
{
  auto const* code = ":::";
  Lexer lexer(code);

  REQUIRE(lexer.peek_token<1>().kind() == Token::Kind::Unknown);
}

TEST_CASE("Returns should lex", "[lexer][lexer_token]")
{
  auto const* code = "->";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Returns, "->");
}

TEST_CASE("End should lex", "[lexer][lexer_token]")
{
  auto const* code = ";";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::End, ";");
}

TEST_CASE("Dot should lex", "[lexer][lexer_token]")
{
  auto const* code = ".";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Dot, ".");
}

TEST_CASE("Comma should lex", "[lexer][lexer_token]")
{
  auto const* code = ",";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Comma, ",");
}

TEST_CASE("Halt should lex continuously", "[lexer][lexer_token]")
{
  auto const* code = "";
  Lexer lexer(code);

  require_next(lexer, Token::Kind::Halt);
  require_next(lexer, Token::Kind::Halt);
  require_next(lexer, Token::Kind::Halt);
}

TEST_CASE("Constant assignment expression should lex", "[lexer][lexer_expression]")
{
  auto const* code = "let x = 12";
  Lexer lexer(code);

  require_sequence(lexer, {
                              {Token::Kind::Keyword, "let"},
                              {Token::Kind::ValueIdentifier, "x"},
                              {Token::Kind::Equal, "="},
                              {Token::Kind::Number, "12"},
                          });
}

TEST_CASE("Compound assignment expression should lex", "[lexer][lexer_expression]")
{
  auto const* code = "let x = y.function()";
  Lexer lexer(code);

  require_sequence(lexer, {
                              {Token::Kind::Keyword, "let"},
                              {Token::Kind::ValueIdentifier, "x"},
                              {Token::Kind::Equal, "="},
                              {Token::Kind::ValueIdentifier, "y"},
                              {Token::Kind::Dot, ""},
                              {Token::Kind::ValueIdentifier, "function"},
                              {Token::Kind::OpenGroup, "("},
                              {Token::Kind::CloseGroup, ")"},
                          });
}

TEST_CASE("Basic main definition should lex", "[lexer][lexer_complex]")
{
  jackal::tests::FileTestResource main("lexer_main.jkl");
  Lexer lexer(main.data());

  require_sequence(lexer, {{Token::Kind::Keyword, "fn"},
                           {Token::Kind::ValueIdentifier, "main"},
                           {Token::Kind::OpenGroup, "("},
                           {Token::Kind::ValueIdentifier, "args"},
                           {Token::Kind::Is, "::"},
                           {Token::Kind::TypeIdentifier, "List"},
                           {Token::Kind::OpenContext, "["},
                           {Token::Kind::TypeIdentifier, "String"},
                           {Token::Kind::CloseContext, "]"},
                           {Token::Kind::CloseGroup, ")"},
                           {Token::Kind::Returns, "->"},
                           {Token::Kind::TypeIdentifier, "Int"},
                           {Token::Kind::OpenScope, "{"},
                           {Token::Kind::Newline, "\n"},
                           {Token::Kind::Keyword, "let"},
                           {Token::Kind::ValueIdentifier, "n"},
                           {Token::Kind::Equal, "="},
                           {Token::Kind::ValueIdentifier, "args"},
                           {Token::Kind::Dot, "."},
                           {Token::Kind::ValueIdentifier, "get"},
                           {Token::Kind::OpenGroup, "("},
                           {Token::Kind::Number, "0"},
                           {Token::Kind::CloseGroup, ")"},
                           {Token::Kind::Dot, "."},
                           {Token::Kind::ValueIdentifier, "parse"},
                           {Token::Kind::OpenContext, "["},
                           {Token::Kind::TypeIdentifier, "Number"},
                           {Token::Kind::CloseContext, "]"},
                           {Token::Kind::OpenGroup, "("},
                           {Token::Kind::CloseGroup, ")"},
                           {Token::Kind::End, ";"},
                           {Token::Kind::Newline, "\n"},
                           {Token::Kind::ValueIdentifier, "write"},
                           {Token::Kind::OpenContext, "["},
                           {Token::Kind::TypeIdentifier, "Stdout"},
                           {Token::Kind::CloseContext, "]"},
                           {Token::Kind::OpenGroup, "("},
                           {Token::Kind::ValueIdentifier, "n"},
                           {Token::Kind::CloseGroup, ")"},
                           {Token::Kind::End, ";"},
                           {Token::Kind::Newline, "\n"},
                           {Token::Kind::Number, "0"},
                           {Token::Kind::Newline, "\n"},
                           {Token::Kind::CloseScope, "}"}});
}

TEST_CASE("Peeking token multiple times should repeatedly return same token", "[lexer]")
{
  auto const* code = "print";
  Lexer lexer(code);

  REQUIRE(lexer.peek_token<0>().kind() == Token::Kind::ValueIdentifier);
  REQUIRE(lexer.peek_token<0>().lexeme() == "print");
}

TEST_CASE("Next after peek should return peeked token", "[lexer]")
{
  auto const* code = "print 456";
  Lexer lexer(code);

  REQUIRE(lexer.peek_token<0>().kind() == Token::Kind::ValueIdentifier);
  REQUIRE(lexer.peek_token<0>().lexeme() == "print");
  require_sequence(lexer, {{Token::Kind::ValueIdentifier, "print"}, {Token::Kind::Number, "456"}});
}

TEST_CASE("Peeking beyond end of code should return halt", "[lexer]")
{
  auto const* code = "whoops";
  Lexer lexer(code);
  REQUIRE(lexer.peek_token<3>().kind() == Token::Kind::Halt);
}

TEST_CASE("Peeking while halted should return halt", "[lexer]")
{
  auto const* code = "";
  Lexer lexer(code);
  REQUIRE(lexer.peek_token<3>().kind() == Token::Kind::Halt);
}

TEST_CASE("Peeking and next should not set halted until halt is consumed", "[lexer]")  // NOLINT
{
  auto const* code = "1 2 3 4";
  Lexer lexer(code);

  REQUIRE(lexer.peek_token<3>().lexeme() == "4");
  REQUIRE(lexer.peek_token<0>().lexeme() == "1");
  REQUIRE(lexer.next().lexeme() == "1");
  REQUIRE(lexer.next().lexeme() == "2");
  REQUIRE(lexer.peek_token<2>().kind() == Token::Kind::Halt);
  REQUIRE(lexer.peek_token<1>().lexeme() == "4");
  REQUIRE_FALSE(lexer.is_halted());
  REQUIRE(lexer.next().lexeme() == "3");
  REQUIRE(lexer.next().lexeme() == "4");
  REQUIRE(lexer.peek_token<0>().kind() == Token::Kind::Halt);
  REQUIRE_FALSE(lexer.is_halted());
  REQUIRE(lexer.next().kind() == Token::Kind::Halt);
  REQUIRE(lexer.is_halted());
}

TEST_CASE("Lexed tokens should have correct source locations",  // NOLINT
          "[lexer][source_location]")
{
  auto const* code = "let x = 123\nlet y = 456\nlet result = x + y\nprint result\n";
  Lexer lexer(code);

  REQUIRE(lexer.peek_token<0>().location().line().num() == 0);
  REQUIRE(lexer.peek_token<0>().location().column() == 0);
  REQUIRE(lexer.peek_token<1>().location().line().num() == 0);
  REQUIRE(lexer.peek_token<1>().location().column() == 4);
  for (auto i = 0; i < 5; ++i)
  {
    lexer.next();
  }
  REQUIRE(lexer.peek_token<3>().location().line().num() == 1);
  REQUIRE(lexer.peek_token<3>().location().column() == 8);
  REQUIRE(lexer.peek_token<0>().location().line().src() == "let y = 456");
}
