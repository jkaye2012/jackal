#pragma once

#include <memory>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Type; }
// clang-format on

namespace jackal::ast
{
struct Expression : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Expression() override;
  Expression(Expression const&) = delete;
  Expression& operator=(Expression const&) = delete;
  Expression(Expression&&) noexcept = delete;
  Expression& operator=(Expression&&) noexcept = delete;

  Type& type() noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};

struct Expressions : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Expressions() override;
  Expressions(Expressions const&) = delete;
  Expressions& operator=(Expressions const&) = delete;
  Expressions(Expressions&&) noexcept = delete;
  Expressions& operator=(Expressions&&) noexcept = delete;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
