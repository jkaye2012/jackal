#include <catch.hpp>

#include "lexer/token.hpp"

using jackal::lexer::Token;

TEST_CASE("Token construction with size should have correct lexeme", "[token]")
{
  auto const* string = "cool string we have here";
  Token token{Token::Kind::Identifier, string, 4};

  REQUIRE(token.kind() == Token::Kind::Identifier);
  REQUIRE(token.lexeme() == "cool");
}

TEST_CASE("Token construction with start and end should have correct lexeme", "[token]")
{
  auto const* string = "awesome message my friend";
  Token token{Token::Kind::Identifier, string, string + 7};

  REQUIRE(token.lexeme() == "awesome");
}
