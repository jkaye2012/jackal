#pragma once

#include <memory>
#include <vector>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Type; }
// clang-format on

namespace jackal::ast
{
struct Context : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { return visitor.visit(*this); }

  ~Context() override;
  Context(Context const&) = delete;
  Context& operator=(Context const&) = delete;
  Context(Context&&) noexcept = delete;
  Context& operator=(Context&&) noexcept = delete;

  std::vector<Type>& types() noexcept;

  [[nodiscard]] bool empty() const noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
