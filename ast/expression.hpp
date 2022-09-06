#pragma once

#include <cassert>
#include <optional>
#include <string_view>
#include <variant>

#include "ast/builder.hpp"
#include "ast/operator.hpp"
#include "ast/value.hpp"

namespace jackal::ast
{
struct Expression : public AbstractSyntaxNode
{
  struct Builder : public AstBuilder
  {
    Operator::Builder op;
    Value::Builder value;

    [[nodiscard]] bool was_modified() const noexcept
    {
      return op.was_modified() || value.was_modified();
    }

    [[nodiscard]] Expression build() const noexcept
    {
      assert(op.was_modified() ^ value.was_modified());
      if (op.was_modified())
      {
        return Expression(op.build());
      }

      return Expression(value.build());
    }
  };

  [[nodiscard]] Operator const& operator_unsafe() const noexcept
  {
    return std::get<Operator>(_expr);
  }

  [[nodiscard]] Value const& value_unsafe() const noexcept { return std::get<Value>(_expr); }

  explicit Expression(Operator op) : _expr(std::move(op)) {}
  explicit Expression(Value value) : _expr(std::move(value)) {}

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] std::variant<Operator, Value>& expression() noexcept { return _expr; }
  [[nodiscard]] std::variant<Operator, Value> const& expression() const noexcept { return _expr; }

 private:
  std::variant<Operator, Value> _expr;
};

struct Binding : public AbstractSyntaxNode
{
  struct Builder : public AstBuilder
  {
    Builder& set_variable(std::string_view name) noexcept
    {
      modified();
      _variable = LocalVariable(name);
      return *this;
    }

    Builder& set_expression(Expression expr) noexcept
    {
      modified();
      _expr = std::move(expr);
      return *this;
    }

    [[nodiscard]] Binding build() const noexcept { return {_variable.value(), _expr.value()}; }

   private:
    std::optional<LocalVariable> _variable;
    std::optional<Expression> _expr;
  };

  Binding(LocalVariable variable, Expression expr) noexcept
      : _variable(std::move(variable)), _expr(std::move(expr))
  {
  }

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] LocalVariable& variable() noexcept { return _variable; }
  [[nodiscard]] LocalVariable const& variable() const noexcept { return _variable; }

  [[nodiscard]] Expression& expression() noexcept { return _expr; }
  [[nodiscard]] Expression const& expression() const noexcept { return _expr; }

 private:
  LocalVariable _variable;
  Expression _expr;
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

    [[nodiscard]] Print build() const noexcept { return Print(_expr.value()); }

   private:
    std::optional<Expression> _expr;
  };

  explicit Print(Expression expr) noexcept : _expr(std::move(expr)) {}

  void accept(Visitor& visitor) noexcept override { visitor.visit(*this); }

  [[nodiscard]] Expression& expression() noexcept { return _expr; }
  [[nodiscard]] Expression const& expression() const noexcept { return _expr; }

 private:
  Expression _expr;
};
}  // namespace jackal::ast
