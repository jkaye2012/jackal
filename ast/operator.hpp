#pragma once

#include <memory>
#include <optional>

#include "ast/builder.hpp"
#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct Expression; }
// clang-format on

namespace jackal::ast
{
struct Operator : public AbstractSyntaxNode
{
  enum class Type
  {
    Add
  };

  struct Builder : public AstBuilder
  {
    Builder() noexcept;
    ~Builder() noexcept;
    Builder(Builder const&) = delete;
    Builder& operator=(Builder const&) = delete;
    Builder(Builder&&) noexcept = delete;
    Builder& operator=(Builder&&) noexcept = delete;

    Builder& set_type(Type type) noexcept;

    Builder& set_a(Expression a) noexcept;

    Builder& set_b(Expression b) noexcept;

    [[nodiscard]] Operator build() noexcept;

   private:
    struct Impl;
    std::unique_ptr<Impl> _impl;
  };

  Operator(Type type, Expression a, Expression b) noexcept;

  ~Operator() override;
  Operator(Operator const&) = delete;
  Operator& operator=(Operator const&) = delete;
  Operator(Operator&&) noexcept;
  Operator& operator=(Operator&&) noexcept;

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] Type type() const noexcept;

  [[nodiscard]] Expression& a() noexcept;
  [[nodiscard]] Expression const& a() const noexcept;

  [[nodiscard]] Expression& b() noexcept;
  [[nodiscard]] Expression const& b() const noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
