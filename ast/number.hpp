#pragma once

#include <cstdint>
#include <memory>

#include "ast/builder.hpp"
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

  explicit Number(int64_t num) noexcept;
  explicit Number(double num) noexcept;

  ~Number() override;
  Number(Number const&) = delete;
  Number& operator=(Number const&) = delete;
  Number(Number&&) noexcept;
  Number& operator=(Number&&) noexcept;

  Type& type() noexcept;

  struct Builder : public AstBuilder
  {
    Builder& value(int64_t num) noexcept;
    Builder& value(double num) noexcept;

    [[nodiscard]] Number build() const noexcept;
  };

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
