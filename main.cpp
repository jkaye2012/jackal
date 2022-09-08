
#include "cli/driver.hpp"
#include "cli/options.hpp"
#include "logger/setup.hpp"

using namespace jackal;  // NOLINT

auto main(int argc, char** argv) -> int
{
  logger::Logger logger{};
  cli::Options options(argc, argv);
  cli::Driver driver(options);
}
