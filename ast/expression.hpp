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

    [[nodiscard]] constexpr bool was_modified() const noexcept
    {
      return op.was_modified() || value.was_modified();
    }

    [[nodiscard]] constexpr Expression build() const noexcept
    {
      assert(op.was_modified() ^ value.was_modified());
      if (op.was_modified())
      {
        return Expression(op.build());
      }

      return Expression(value.build());
    }
  };

  [[nodiscard]] constexpr Operator const& operator_unsafe() const noexcept
  {
    return std::get<Operator>(_expr);
  }
  [[nodiscard]] constexpr Value const& value_unsafe() const noexcept
  {
    return std::get<Value>(_expr);
  }

  explicit constexpr Expression(Operator op) : _expr(std::move(op)) {}
  explicit constexpr Expression(Value value) : _expr(std::move(value)) {}

 private:
  std::variant<Operator, Value> _expr;
};

struct Binding : public AbstractSyntaxNode
{
  struct Builder : public AstBuilder
  {
    constexpr Builder& set_variable(std::string_view name) noexcept
    {
      modified();
      _variable = LocalVariable(name);
      return *this;
    }

    constexpr Builder& set_expression(Expression expr) noexcept
    {
      modified();
      _expr = std::move(expr);
      return *this;
    }

    [[nodiscard]] constexpr Binding build() const noexcept
    {
      return Binding(_variable.value(), _expr.value());
    }

   private:
    std::optional<LocalVariable> _variable;
    std::optional<Expression> _expr;
  };

  constexpr Binding(LocalVariable variable, Expression expr) noexcept
      : _variable(std::move(variable)), _expr(std::move(expr))
  {
  }

  [[nodiscard]] constexpr LocalVariable const& variable() const noexcept { return _variable; }
  [[nodiscard]] constexpr Expression const& expression() const noexcept { return _expr; }

 private:
  LocalVariable _variable;
  Expression _expr;
};

struct Print : public AbstractSyntaxNode
{
  struct Builder : public AstBuilder
  {
    constexpr Builder& set_expression(Expression expr) noexcept
    {
      modified();
      _expr = std::move(expr);
      return *this;
    }

    [[nodiscard]] constexpr Print build() const noexcept { return Print(_expr.value()); }

   private:
    std::optional<Expression> _expr;
  };

  explicit constexpr Print(Expression expr) noexcept : _expr(std::move(expr)) {}

  [[nodiscard]] constexpr Expression const& expression() const noexcept { return _expr; }

 private:
  Expression _expr;
};
}  // namespace jackal::ast
