#pragma once

#include <cstdlib>

namespace jackal::util
{
static constexpr auto ExitInvalidArguments = 255;
static constexpr auto ExitMissingSource = 254;
static constexpr auto ExitCouldNotCreateTempDir = 253;
static constexpr auto ExitSyntaxError = 252;
static constexpr auto ExitCodeGenerationFailed = 251;
}  // namespace jackal::util
