#pragma once

#include <functional>

// clang-format off
namespace jackal::ast { struct Visitor; }
// clang-format on

namespace jackal::ast
{
template <typename T>
using Ref = std::reference_wrapper<T>;

struct AbstractSyntaxNode
{
  virtual constexpr ~AbstractSyntaxNode() = default;

  virtual void accept(Visitor& visitor) noexcept = 0;

  // TODO: should Type be required of all syntax nodes? Thinking yes
};
}  // namespace jackal::ast
