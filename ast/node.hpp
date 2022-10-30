#pragma once

#include <functional>

#include "ast/visitor.hpp"

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

  // TODO: provide default implementation here, remove override from most nodes
  virtual void accept(Visitor& visitor) noexcept = 0;

  // TODO: should Type be required of all syntax nodes? Thinking yes
};
}  // namespace jackal::ast
