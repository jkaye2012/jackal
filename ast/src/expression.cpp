#include "ast/expression.hpp"

#include <memory>
#include <variant>

#include "ast/operator.hpp"
#include "ast/value.hpp"

using jackal::ast::Binding;
using jackal::ast::Expression;
using jackal::ast::Print;

struct Expression::Builder::Impl
{
  std::variant<std::monostate, Operator, Value> expr;
};

Expression::Builder::Builder() noexcept : _impl(std::make_unique<Impl>()) {}

Expression::Builder::~Builder() noexcept = default;

auto Expression::Builder::was_modified() const noexcept -> bool { return _impl->expr.index() > 0; }

auto Expression::Builder::build() noexcept -> Expression
{
  assert(was_modified());
  if (auto* op = std::get_if<Operator>(&_impl->expr))
  {
    return Expression(std::move(*op));
  }
  if (auto* val = std::get_if<Value>(&_impl->expr))
  {
    return Expression(std::move(*val));
  }

  std::terminate();
}

struct Expression::Impl
{
  std::variant<Operator, Value> expr;
};

Expression::Expression(Operator op) : _impl(std::make_unique<Impl>(Impl{std::move(op)})) {}

Expression::Expression(Value value) : _impl(std::make_unique<Impl>(Impl{std::move(value)})) {}

Expression::~Expression() = default;
Expression::Expression(Expression&&) noexcept = default;
Expression& Expression::operator=(Expression&&) noexcept = default;

auto Expression::operator_unsafe() const noexcept -> Operator const&
{
  return std::get<Operator>(_impl->expr);
}

auto Expression::value_unsafe() const noexcept -> Value const&
{
  return std::get<Value>(_impl->expr);
}

auto Expression::expression() noexcept -> std::variant<Operator, Value>& { return _impl->expr; }

auto Expression::expression() const noexcept -> std::variant<Operator, Value> const&
{
  return _impl->expr;
}

struct Binding::Builder::Impl
{
  std::optional<LocalVariable> variable;
  std::optional<Expression> expr;
};

Binding::Builder::Builder() noexcept : _impl(std::make_unique<Impl>()) {}

Binding::Builder::~Builder() noexcept = default;

auto Binding::Builder::set_variable(std::string_view name) noexcept -> Builder&
{
  modified();
  _impl->variable = LocalVariable(name);
  return *this;
}

auto Binding::Builder::set_expression(Expression expr) noexcept -> Builder&
{
  modified();
  _impl->expr = std::move(expr);
  return *this;
}

auto Binding::Builder::build() noexcept -> Binding
{
  return {std::move(_impl->variable.value()), std::move(_impl->expr.value())};
}

struct Binding::Impl
{
  LocalVariable variable;
  Expression expr;
};

Binding::Binding(LocalVariable variable, Expression expr) noexcept
    : _impl(std::make_unique<Impl>(Impl{std::move(variable), std::move(expr)}))
{
}

Binding::~Binding() = default;
Binding::Binding(Binding&&) noexcept = default;
Binding& Binding::operator=(Binding&&) noexcept = default;

auto Binding::variable() noexcept -> LocalVariable& { return _impl->variable; }
auto Binding::variable() const noexcept -> LocalVariable const& { return _impl->variable; }

auto Binding::expression() noexcept -> Expression& { return _impl->expr; }
auto Binding::expression() const noexcept -> Expression const& { return _impl->expr; }

struct Print::Impl
{
  Expression expr;
};

Print::Print(Expression expr) noexcept : _impl(std::make_unique<Impl>(Impl{std::move(expr)})) {}

Print::~Print() = default;
Print::Print(Print&&) noexcept = default;
Print& Print::operator=(Print&&) noexcept = default;

auto Print::expression() noexcept -> Expression& { return _impl->expr; }
auto Print::expression() const noexcept -> Expression const& { return _impl->expr; }
