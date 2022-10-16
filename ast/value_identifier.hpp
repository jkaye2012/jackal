#pragma once

#include <string_view>

namespace jackal::ast
{
struct ValueIdentifier
{
  explicit ValueIdentifier(std::string_view name) noexcept : _name(name) {}

  auto operator<=>(ValueIdentifier const&) const noexcept = default;

 private:
  std::string_view _name;
};
}  // namespace jackal::ast
