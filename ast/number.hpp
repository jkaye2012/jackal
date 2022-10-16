#pragma once

#include <memory>

#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Type; }
// clang-format on

namespace jackal::ast
{
struct Number : public AbstractSyntaxNode
{
  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  ~Number() override;
  Number(Number const&) = delete;
  Number& operator=(Number const&) = delete;
  Number(Number&&) noexcept = delete;
  Number& operator=(Number&&) noexcept = delete;

  Type& type() noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
