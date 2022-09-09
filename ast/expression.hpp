#pragma once

#include <cassert>
#include <memory>
#include <optional>
#include <string_view>
#include <variant>

#include "ast/builder.hpp"
#include "ast/node.hpp"
#include "ast/visitor.hpp"

// clang-format off
namespace jackal::ast { struct LocalVariable; }
namespace jackal::ast { struct Operator; }
namespace jackal::ast { struct Value; }
// clang-format on

namespace jackal::ast
{
struct Expression : public AbstractSyntaxNode
{
  struct Builder : public AstBuilder
  {
    Builder() noexcept;
    ~Builder() noexcept;
    Builder(Builder const&) = delete;
    Builder& operator=(Builder const&) = delete;
    Builder(Builder&&) noexcept = delete;
    Builder& operator=(Builder&&) noexcept = delete;

    [[nodiscard]] bool was_modified() const noexcept;

    [[nodiscard]] Expression build() noexcept;

   private:
    struct Impl;
    std::unique_ptr<Impl> _impl;
  };

  explicit Expression(Operator op);
  explicit Expression(Value value);

  ~Expression() override;
  Expression(Expression const&) = delete;
  Expression& operator=(Expression const&) = delete;
  Expression(Expression&&) noexcept;
  Expression& operator=(Expression&&) noexcept;

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] Operator const& operator_unsafe() const noexcept;

  [[nodiscard]] Value const& value_unsafe() const noexcept;

  [[nodiscard]] std::variant<Operator, Value>& expression() noexcept;
  [[nodiscard]] std::variant<Operator, Value> const& expression() const noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};

struct Binding : public AbstractSyntaxNode
{
  struct Builder : public AstBuilder
  {
    Builder() noexcept;
    ~Builder() noexcept;
    Builder(Builder const&) = delete;
    Builder& operator=(Builder const&) = delete;
    Builder(Builder&&) noexcept = delete;
    Builder& operator=(Builder&&) noexcept = delete;

    Builder& set_variable(std::string_view name) noexcept;

    Builder& set_expression(Expression expr) noexcept;

    [[nodiscard]] Binding build() noexcept;

   private:
    struct Impl;
    std::unique_ptr<Impl> _impl;
  };

  Binding(LocalVariable variable, Expression expr) noexcept;

  ~Binding() override;
  Binding(Binding const&) = delete;
  Binding& operator=(Binding const&) = delete;
  Binding(Binding&&) noexcept;
  Binding& operator=(Binding&&) noexcept;

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] LocalVariable& variable() noexcept;
  [[nodiscard]] LocalVariable const& variable() const noexcept;

  [[nodiscard]] Expression& expression() noexcept;
  [[nodiscard]] Expression const& expression() const noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};

struct Print : public AbstractSyntaxNode
{
  struct Builder : public AstBuilder
  {
    Builder& set_expression(Expression expr) noexcept
    {
      modified();
      _expr = std::move(expr);
      return *this;
    }

    [[nodiscard]] Print build() noexcept { return Print(std::move(_expr.value())); }

   private:
    std::optional<Expression> _expr;
  };

  explicit Print(Expression expr) noexcept;

  ~Print() override;
  Print(Print const&) = delete;
  Print& operator=(Print const&) = delete;
  Print(Print&&) noexcept;
  Print& operator=(Print&&) noexcept;

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] Expression& expression() noexcept;
  [[nodiscard]] Expression const& expression() const noexcept;

 private:
  struct Impl;
  std::unique_ptr<Impl> _impl;
};
}  // namespace jackal::ast
