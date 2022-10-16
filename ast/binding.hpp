#pragma once

#include <memory>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Expression; }
namespace jackal::ast { struct Variable; }
// clang-format on

namespace jackal::ast
{
struct Binding : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Binding() override;
  Binding(Binding const&) = delete;
  Binding& operator=(Binding const&) = delete;
  Binding(Binding&&) noexcept = delete;
  Binding& operator=(Binding&&) noexcept = delete;

  Variable& variable() noexcept;

  Expression& expression() noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
