#include <catch.hpp>

#include "lexer/token.hpp"
#include "util/source_location.hpp"

using jackal::lexer::Token;

constexpr jackal::util::SourceLocation kLoc{jackal::util::Line("testing", 0),
                                            jackal::util::column(0)};

TEST_CASE("Token construction with size should have correct lexeme", "[token]")
{
  auto const* string = "cool string we have here";
  Token token{Token::Kind::Identifier, kLoc, string, 4};

  REQUIRE(token.kind() == Token::Kind::Identifier);
  REQUIRE(token.lexeme() == "cool");
}

TEST_CASE("Token construction with start and end should have correct lexeme", "[token]")
{
  auto const* string = "awesome message my friend";
  Token token{Token::Kind::Identifier, kLoc, string, string + 7};

  REQUIRE(token.lexeme() == "awesome");
}
