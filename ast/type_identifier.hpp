#pragma once

#include <string_view>

namespace jackal::ast
{
struct TypeIdentifier
{
  explicit TypeIdentifier(std::string_view name) noexcept : _name(name) {}

  auto operator<=>(TypeIdentifier const&) const noexcept = default;

 private:
  std::string_view _name;
};
}  // namespace jackal::ast
