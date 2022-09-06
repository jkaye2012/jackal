#include <catch.hpp>

#include <optional>

#include "util/exec.hpp"

TEST_CASE("util::exec should return stdout of successful command", "[exec]")
{
  auto result = jackal::util::exec("echo 'this is a command'");
  REQUIRE(result.has_value());
  REQUIRE(*result == "this is a command\n");
}

TEST_CASE("util::exec should return nullopt when a command fails", "[exec]")
{
  auto result = jackal::util::exec("exit 1");
  REQUIRE(!result.has_value());
}
