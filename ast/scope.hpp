#pragma once

#include <memory>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Expressions; }
namespace jackal::ast { struct Type; }
// clang-format on

namespace jackal::ast
{
struct Scope : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Scope() override;
  Scope(Scope const&) = delete;
  Scope& operator=(Scope const&) = delete;
  Scope(Scope&&) noexcept = delete;
  Scope& operator=(Scope&&) noexcept = delete;

  Type& type() noexcept;

  Expressions& body() noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
