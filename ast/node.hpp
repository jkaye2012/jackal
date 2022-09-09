#pragma once

// clang-format off
namespace jackal::ast { struct Visitor; }
// clang-format on

namespace jackal::ast
{
struct AbstractSyntaxNode
{
  virtual constexpr ~AbstractSyntaxNode() = default;

  virtual void accept(Visitor& visitor) noexcept = 0;
};
}  // namespace jackal::ast
