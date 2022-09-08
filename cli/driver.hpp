#pragma once

// clang-format off
namespace jackal::cli { struct Options; }
// clang-format on

namespace jackal::cli
{
struct Driver
{
  explicit Driver(Options const& options) noexcept;
};
}  // namespace jackal::cli
