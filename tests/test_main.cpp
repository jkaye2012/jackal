#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

auto main(int argc, char** argv) -> int { return Catch::Session().run(argc, argv); }
