#pragma once

// clang-format off
namespace jackal::ast { struct Visitor; }
// clang-format on

// To continue: pimpl-ize all AST nodes to validate that this allows for compound
// arithmetic expressions. Generally, this will add some overhead to the compilation
// process (because pointers into each node will have to be chased), but it should also
// allow general composition of nodes without worry of concrete memory format (e.g.
// circumventing the current circular reference between Expresison and Operator).

namespace jackal::ast
{
struct AbstractSyntaxNode
{
  virtual constexpr ~AbstractSyntaxNode() = default;

  virtual void accept(Visitor& visitor) noexcept = 0;
};
}  // namespace jackal::ast
