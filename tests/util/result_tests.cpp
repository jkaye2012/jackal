#include <catch.hpp>

#include <functional>
#include <string>

#include "util/result.hpp"

using Result = jackal::util::Result<int, std::string>;

static std::function<int(int const&)> const same_type_mapper = [](int i)
{
  return i * 2;
};

static std::function<double(int const&)> const different_type_mapper = [](int i)
{
  return 0.5 * i;
};

static std::function<std::string(std::string const&)> const error_mapper = [](std::string const& e)
{
  return e.substr(3);
};

static std::function<void(int&, int const&)> const subsumer = [](int& i, int const& other)
{
  i *= other;
};

TEST_CASE("Result constructed from ok value should be ok", "[result]")
{
  auto result = Result::from(12190);
  REQUIRE(result.is_ok());
}

TEST_CASE("Result constructed from err value should be err", "[result]")
{
  auto result = Result::from("oh no");
  REQUIRE(result.is_err());
}

TEST_CASE("Result should be able to return ok value", "[result]")
{
  auto result = Result::from(81590);
  REQUIRE(result.ok() == 81590);
}

TEST_CASE("Result should be able to return err value", "[result]")
{
  auto result = Result::from("foobar");
  REQUIRE(result.err() == "foobar");
}

TEST_CASE("Result::map with ok value should return a transformed Result", "[result]")
{
  auto result = Result::from(123);
  REQUIRE(result.map(same_type_mapper).ok() == 246);
}

TEST_CASE("Result::map_err with err value should return a transformed Result", "[result]")
{
  auto result = Result::from("foobar");
  REQUIRE(result.map_err(error_mapper).err() == "bar");
}

TEST_CASE("Result::map_flat with ok value should return a transformed Result", "[result]")
{
  auto result = Result::from(10);
  REQUIRE(result.map_flat(different_type_mapper, error_mapper).ok() == Approx(5.0));
}

TEST_CASE("Result::map_flat with err value should return a transformed Result", "[result]")
{
  auto result = Result::from("baderror");
  REQUIRE(result.map_flat(different_type_mapper, error_mapper).err() == "error");
}

TEST_CASE("Result::subsume with err value should do nothing", "[result]")
{
  auto result = Result::from("very sad");
  auto other = Result::from("could be anything really");
  result.subsume(subsumer, other);
  REQUIRE(result.err() == "very sad");
}

TEST_CASE("Result::subsume with ok value should propagate an err Result", "[result]")
{
  auto result = Result::from(50);
  auto other = Result::from("failure");
  result.subsume(subsumer, other);
  REQUIRE(result.err() == "failure");
}

TEST_CASE("Result::subsume with ok value should subsume an ok Result", "[result]")
{
  auto result = Result::from(50);
  auto other = Result::from(2);
  result.subsume(subsumer, other);
  REQUIRE(result.ok() == 100);
}
