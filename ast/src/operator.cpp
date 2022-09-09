#include "ast/operator.hpp"

#include <memory>

#include "ast/expression.hpp"

using jackal::ast::Operator;

struct Operator::Builder::Impl
{
  std::optional<Type> type;
  std::optional<Expression> a;
  std::optional<Expression> b;
};

Operator::Builder::Builder() noexcept : _impl(std::make_unique<Impl>()) {}
Operator::Builder::~Builder() noexcept = default;

auto Operator::Builder::set_type(Type type) noexcept -> Builder&
{
  modified();
  _impl->type = type;
  return *this;
}

auto Operator::Builder::set_a(Expression a) noexcept -> Builder&
{
  modified();
  _impl->a = std::move(a);
  return *this;
}

auto Operator::Builder::set_b(Expression b) noexcept -> Builder&
{
  modified();
  _impl->b = std::move(b);
  return *this;
}

auto Operator::Builder::build() noexcept -> Operator
{
  return {_impl->type.value(), std::move(_impl->a.value()), std::move(_impl->b.value())};
}

struct Operator::Impl
{
  Type type;
  Expression a;
  Expression b;
};

Operator::Operator(Type type, Expression a, Expression b) noexcept
    : _impl(std::make_unique<Impl>(Impl{type, std::move(a), std::move(b)}))
{
}

Operator::~Operator() = default;
Operator::Operator(Operator&&) noexcept = default;
Operator& Operator::operator=(Operator&&) noexcept = default;

auto Operator::type() const noexcept -> Type { return _impl->type; }

auto Operator::a() noexcept -> Expression& { return _impl->a; }
auto Operator::a() const noexcept -> Expression const& { return _impl->a; }

auto Operator::b() noexcept -> Expression& { return _impl->b; }
auto Operator::b() const noexcept -> Expression const& { return _impl->b; }
