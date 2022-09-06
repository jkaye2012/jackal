#include <catch.hpp>

#include <filesystem>

#include "util/file_system.hpp"

using jackal::util::TemporaryDirectory;

TEST_CASE("TemporaryDirectory should create and remove random directory on disk", "[filesystem]")
{
  std::filesystem::path path;

  {
    auto temp = TemporaryDirectory();
    path = temp.directory();
    REQUIRE(std::filesystem::exists(path));
  }

  REQUIRE(!std::filesystem::exists(path));
}
