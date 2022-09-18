#include <catch.hpp>

#include "util/source_location.hpp"

using jackal::util::column;
using jackal::util::Line;
using jackal::util::SourceLocation;

TEST_CASE("Source location should construct with correct line and column information",
          "[source_location]")
{
  char const* source = "this could be a program\nbut it's probably not\n";
  SourceLocation loc(Line(source, 0), 0);

  REQUIRE(loc.column() == 0);
  REQUIRE(loc.line().src().data() == source);
  REQUIRE(loc.line().num() == 0);
}

TEST_CASE("Source location to_string should return debug string", "[source_location]")
{
  char const* source = "let this = reasonable\nlet that = broken\nlet test = passing";
  SourceLocation loc(Line(source, 0), 4);

  REQUIRE(jackal::util::to_string(loc) == "let this = reasonable\n    ^\n    └----");
}
